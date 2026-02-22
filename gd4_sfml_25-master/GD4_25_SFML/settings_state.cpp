//edited by Jopseph Byrne
#include "settings_state.hpp"
#include "Utility.hpp"

static bool IsPlayer2Action(Action action)
{
    return action == Action::kMoveLeftPlayer2 || action == Action::kMoveRightPlayer2 || action == Action::kSpawnAlienPlayer2 || action == Action::kSpawnAlien2Player2 || action == Action::kSpawnAlien3Player2;
}

SettingsState::SettingsState(StateStack& stack, Context context)
    : State(stack, context)
    , m_gui_container()
    , m_background_sprite(context.textures->Get(TextureID::kAltBackground))
    //added title to the settings menu
    , m_title_text(context.fonts->Get(FontID::kMain))
{
    //setting up the title for the settings menu
    m_title_text.setString("Player Controls");
    m_title_text.setCharacterSize(70);
    m_title_text.setFillColor(sf::Color::Black);
    Utility::CentreOrigin(m_title_text);
    m_title_text.setPosition(sf::Vector2f(512.f, 100.f));

    //removed from the game as no up down movement.
    //AddButtonLabel(Action::kMoveUp, 150.f, "Move Up", context);
    //AddButtonLabel(Action::kMoveDown, 200.f, "Move Down", context);
    AddButtonLabel(Action::kMoveRight, 200.f, 80.f, "Move Right", context);
    AddButtonLabel(Action::kMoveLeft, 250.f, 80.f, "Move Left", context);
    AddButtonLabel(Action::kBulletFire, 300.f, 80.f, "Fire", context);
    AddButtonLabel(Action::kMissileFire, 350.f, 80.f, "Missile Fire", context);

    //Added the player 2 controls to the settings menu with the same format as the player 1 controls - also moved them right so that they are sepereate on screen.
	AddButtonLabel(Action::kMoveRightPlayer2, 200.f, 650.f, "Move Right Player 2", context);
	AddButtonLabel(Action::kMoveLeftPlayer2, 250.f, 650.f, "Move Left Player 2", context);
	AddButtonLabel(Action::kSpawnAlienPlayer2, 300.f, 650.f, "Spawn Alien Ship", context);
	AddButtonLabel(Action::kSpawnAlien2Player2, 350.f, 650.f, "Spawn Alien Ship 2", context);
	AddButtonLabel(Action::kSpawnAlien3Player2, 400.f, 650.f, "Spawn Alien Ship 3", context);

    UpdateLabels();

    // added the music and sfx toggle buttons to the settings menu, which toggles the music and sfx on and off and changes the text on the button to reflect the state of the music and sfx. ***(ChatGPT was used to help with the implementation)***
    auto music_button = std::make_shared<gui::Button>(*context.fonts, *context.textures, *context.audio);//added the audio context to all the button constructors so they make a sound when clicked.
    music_button->setPosition(sf::Vector2f(80.f, 475.f));
    music_button->SetText("Music: ON");
    music_button->SetToggle(true);
    music_button->SetCallback([music_button, audio = context.audio]()
        {
            audio->toggle_music();
            //updates the text n the button to show what state it is in.
            music_button->SetText(audio->is_music_on() ? "Music: ON" : "Music: OFF");
        });
    m_gui_container.Pack(music_button);

    auto sfx_button = std::make_shared<gui::Button>(*context.fonts, *context.textures, *context.audio);//added the audio context to all the button constructors so they make a sound when clicked.
    sfx_button->setPosition(sf::Vector2f(80.f, 525.f));
    sfx_button->SetText("SFX: ON");
    sfx_button->SetToggle(true);
    sfx_button->SetCallback([sfx_button, audio = context.audio]()
        {
            audio->toggle_sound();
            //updates the text n the button to show what state it is in.
            sfx_button->SetText(audio->is_sound_on() ? "SFX: ON" : "SFX: OFF");
        });
    m_gui_container.Pack(sfx_button);


	auto back_button = std::make_shared<gui::Button>(*context.fonts, *context.textures, *context.audio);//added the audio context to all the button constructors so they make a sound when clicked.
    back_button->setPosition(sf::Vector2f(80.f, 575.f));
    back_button->SetText("Back");
    back_button->SetCallback(std::bind(&SettingsState::RequestStackPop, this));
    m_gui_container.Pack(back_button);
}

void SettingsState::Draw()
{
    sf::RenderWindow& window = *GetContext().window;
    window.draw(m_background_sprite);
	window.draw(m_title_text);
    window.draw(m_gui_container);
}

bool SettingsState::Update(sf::Time dt)
{
    return true;
}

bool SettingsState::HandleEvent(const sf::Event& event)
{
    bool is_key_binding = false;

    //Iterate through all of the key binding buttons to see if they are being pressed, waiting for input from the user
    for (std::size_t action = 0; action < static_cast<int>(Action::kActionCount); ++action)
    {
        if (m_binding_buttons[action]->IsActive())
        {
            is_key_binding = true;
            const auto* key_released = event.getIf<sf::Event::KeyReleased>();
            if (key_released)
            {
                Action a = static_cast<Action>(action);
                if (IsPlayer2Action(a))
                    GetContext().player_2->AssignKey(a, key_released->scancode);
                else
                    GetContext().player->AssignKey(a, key_released->scancode);
                m_binding_buttons[action]->Deactivate();
            }
            break;
        }
    }

    if (is_key_binding)
    {
        UpdateLabels();
    }
    else
    {
        m_gui_container.HandleEvent(event);
    }
    return false;
}

void SettingsState::UpdateLabels()
{
    Player& p1 = *GetContext().player;
    //updating the labels for player 2 as well as player 1 to show the current key bindings for each.
    Player& p2 = *GetContext().player_2;
    for (std::size_t i = 0; i < static_cast<int>(Action::kActionCount); ++i)
    {
        if (m_binding_labels[i])
        {
            //get the assigned key for the action and update the label to show what the new keybind is.
            Action a = static_cast<Action>(i);
            sf::Keyboard::Scancode key = IsPlayer2Action(a) ? p2.GetAssignedKey(a) : p1.GetAssignedKey(a);
            m_binding_labels[i]->SetText(Utility::toString(key));
        }
    }
}

void SettingsState::AddButtonLabel(Action action, float y, float x, const std::string& text, Context context)
{
    m_binding_buttons[static_cast<int>(action)] = std::make_shared<gui::Button>(*context.fonts, *context.textures, *context.audio);//added the audio context to all the button constructors so they make a sound when clicked.
    m_binding_buttons[static_cast<int>(action)]->setPosition(sf::Vector2f(x,y));
    m_binding_buttons[static_cast<int>(action)]->SetText(text);
    m_binding_buttons[static_cast<int>(action)]->SetToggle(true);

    m_binding_labels[static_cast<int>(action)] = std::make_shared<gui::Label>("",  * context.fonts);
    //changed how much the label is moved to the right.
    m_binding_labels[static_cast<int>(action)]->setPosition(sf::Vector2f(x + 225.f, y + 15.f));

    m_gui_container.Pack(m_binding_buttons[static_cast<int>(action)]);
    m_gui_container.Pack(m_binding_labels[static_cast<int>(action)]);
}
