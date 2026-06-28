#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(EditorUI) {
    struct Fields {
        bool m_isSavingAndExiting = false;
        CCObject* m_sender = nullptr;
    };

    void onExitEditor(CCObject* sender) {
        checkAndRename(false, sender);
    }

    void onSaveAndExit(CCObject* sender) {
        checkAndRename(true, sender);
    }

    void checkAndRename(bool saveAndExit, CCObject* sender) {
        auto level = this->m_editorLayer->m_level;
        if (!level) {
            if (saveAndExit) {
                EditorUI::onSaveAndExit(sender);
            } else {
                EditorUI::onExitEditor(sender);
            }
            return;
        }

        // the checks
        auto levelName = level->m_levelName;
        if (!levelName.starts_with("unnamed") || (level->m_songID == 0 && level->m_audioTrack == 1)) {
            if (saveAndExit) {
                EditorUI::onSaveAndExit(sender);
            } else {
                EditorUI::onExitEditor(sender);
            }
            return;
        }

        auto songName = level->getSongName();

        // fuck special chars
        std::string cleanedName;
        for (char c : songName) {
            if (std::isalnum(static_cast<unsigned char>(c)) || c == ' ') {
                cleanedName += c;
            }
        }

        size_t start = cleanedName.find_first_not_of(' ');
        size_t end = cleanedName.find_last_not_of(' ');
        if (start != std::string::npos && end != std::string::npos) {
            cleanedName = cleanedName.substr(start, end - start + 1);
        }

        if (cleanedName.empty()) {
            if (saveAndExit) {
                EditorUI::onSaveAndExit(sender);
            } else {
                EditorUI::onExitEditor(sender);
            }
            return;
        }

        m_fields->m_isSavingAndExiting = saveAndExit;
        m_fields->m_sender = sender;

        // popup
        createQuickPopup("Level Name Suggestion", fmt::format("Do you want to rename the level to \"{}\"?", cleanedName), "No", "Yes", [this, level, cleanedName](auto, bool btn2) {
                if (btn2) {
                    level->m_levelName = cleanedName;
                }
                if (m_fields->m_isSavingAndExiting) {
                    EditorUI::onSaveAndExit(m_fields->m_sender);
                } else {
                    EditorUI::onExitEditor(m_fields->m_sender);
                }
            }
        );
    }
};
