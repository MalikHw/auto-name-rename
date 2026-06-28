#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(AutoRenameEditorUI, EditorUI) {
    struct Fields {
        bool m_isSavingAndExiting = false;
        CCObject* m_sender = nullptr;
        bool m_hasShownPopup = false;
    };

    void onExitEditor(CCObject* sender) {
        if (!m_fields->m_hasShownPopup) {
            checkAndRename(false, sender);
        } else {
            m_fields->m_hasShownPopup = false;
            AutoRenameEditorUI::onExitEditor(sender);
        }
    }

    void onSaveAndExit(CCObject* sender) {
        if (!m_fields->m_hasShownPopup) {
            checkAndRename(true, sender);
        } else {
            m_fields->m_hasShownPopup = false;
            AutoRenameEditorUI::onSaveAndExit(sender);
        }
    }

    void checkAndRename(bool saveAndExit, CCObject* sender) {
        auto level = this->m_editorLayer->m_level;
        if (!level) {
            if (saveAndExit) {
                AutoRenameEditorUI::onSaveAndExit(sender);
            } else {
                AutoRenameEditorUI::onExitEditor(sender);
            }
            return;
        }

        // the checks
        std::string levelName = std::string(level->m_levelName);
        if (!(levelName.find("unnamed") == 0) || (level->m_songID == 0 && level->m_audioTrack == 1)) {
            if (saveAndExit) {
                AutoRenameEditorUI::onSaveAndExit(sender);
            } else {
                AutoRenameEditorUI::onExitEditor(sender);
            }
            return;
        }

        std::string songName = std::string(level->getSongName());

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
                AutoRenameEditorUI::onSaveAndExit(sender);
            } else {
                AutoRenameEditorUI::onExitEditor(sender);
            }
            return;
        }

        m_fields->m_isSavingAndExiting = saveAndExit;
        m_fields->m_sender = sender;
        m_fields->m_hasShownPopup = true;

        // popup
                if (btn2) {
                    level->m_levelName = cleanedName;
                }
                if (m_fields->m_isSavingAndExiting) {
                    AutoRenameEditorUI::onSaveAndExit(m_fields->m_sender);
                } else {
                    AutoRenameEditorUI::onExitEditor(m_fields->m_sender);
                }
            }
        );
    }
};
