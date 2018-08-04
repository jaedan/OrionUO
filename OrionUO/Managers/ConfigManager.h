

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

class CConfigManager
{
protected:
    bool m_Sound = true;
    bool m_Music = false;
    uchar m_SoundVolume = 255;
    uchar m_MusicVolume = 255;
    uchar m_ClientFPS = 60;
    uchar m_DrawStatusState = 0;
    bool m_DrawStumps = false;
    bool m_MarkingCaves = false;
    bool m_NoVegetation = false;
    bool m_NoAnimateFields = false;
    bool m_ConsoleNeedEnter = false;
    uchar m_SpellIconAlpha = 0;
    bool m_OldStyleStatusbar = false;
    bool m_OriginalPartyStatusbar = false;
    bool m_ApplyStateColorOnCharacters = false;
    bool m_ChangeFieldsGraphic = false;
    bool m_PaperdollSlots = false;
    uchar m_DrawAuraState = 0;
    bool m_ScaleImagesInPaperdollSlots = false;
    bool m_UseGlobalMapLayer = false;
    bool m_NoDrawRoofs = false;
    bool m_UseGLListsForInterface = false;
    uchar m_PingTimer = 10;
    uchar m_ItemPropertiesMode = OPM_FOLLOW_MOUSE;
    bool m_ItemPropertiesIcon = false;
    uchar m_CharacterBackpackStyle = CBS_DEFAULT;

public:
    bool FootstepsSound = true;
    bool CombatMusic = true;
    bool BackgroundSound = true;
    bool AutomaticallyOpenDoors = true;
    bool RemoveTextWithBlending = false;
    uchar DrawStatusConditionState = 0;
    uchar DrawStatusConditionValue = 0;
    bool LockGumpsMoving = false;
    uchar HiddenCharactersRenderMode = 0;
    uchar HiddenAlpha = 0;
    bool UseHiddenModeOnlyForSelf = false;
    uchar TransparentSpellIcons = 0;
    bool RemoveStatusbarsWithoutObjects = false;
    bool ShowDefaultConsoleEntryMode = false;
    bool DrawAuraWithCtrlPressed = false;
    uchar ScreenshotFormat = 0;
    bool RemoveOrCreateObjectsWithBlending = false;
    bool DrawHelmetsOnShroud = false;
    bool HighlightTargetByType = true;
    bool AutoDisplayWorldMap = false;
    bool CheckPing = true;
    bool CancelNewTargetSystemOnShiftEsc = false;
    bool DrawStatusForHumanoids = true;

    bool UseToolTips = false;
    ushort ToolTipsTextColor = 0;
    ushort ToolTipsTextFont = 0;
    ushort ToolTipsDelay = 0;

    ushort ChatColorInputText = 0;
    ushort ChatColorMenuOption = 0;
    ushort ChatColorPlayerInMemberList = 0;
    ushort ChatColorText = 0;
    ushort ChatColorPlayerNameWithout = 0;
    ushort ChatColorMuted = 0;
    ushort ChatColorChannelModeratorName = 0;
    ushort ChatColorChannelModeratorText = 0;
    ushort ChatColorMyName = 0;
    ushort ChatColorMyText = 0;
    ushort ChatColorSystemMessage = 0;
    ushort ChatFont = 0;
    ushort ChatColorBGOutputText = 0;
    ushort ChatColorBGInputText = 0;
    ushort ChatColorBGUserList = 0;
    ushort ChatColorBGConfList = 0;
    ushort ChatColorBGCommandList = 0;

    bool EnablePathfind = false;
    bool HoldTabForCombat = false;
    bool OffsetInterfaceWindows = false;
    bool AutoArrange = false;
    bool AlwaysRun = false;
    bool DisableMenubar = false;
    bool GrayOutOfRangeObjects = false;
    bool DisableNewTargetSystem = false;
    bool HoldShiftForContextMenus = false;
    bool HoldShiftForEnablePathfind = false;
    int GameWindowWidth = 0;
    int GameWindowHeight = 0;
    ushort SpeechDelay = 0;
    bool ScaleSpeechDelay = false;
    ushort SpeechColor = 0;
    ushort EmoteColor = 0;
    ushort PartyMessageColor = 0;
    ushort GuildMessageColor = 0;
    ushort AllianceMessageColor = 0;
    bool IgnoreGuildMessage = false;
    bool IgnoreAllianceMessage = false;
    ushort LightLevel = 80;
    bool LockResizingGameWindow = false;

    ushort InnocentColor = 0;
    ushort FriendlyColor = 0;
    ushort SomeoneColor = 0;
    ushort CriminalColor = 0;
    ushort EnemyColor = 0;
    ushort MurdererColor = 0;
    bool CriminalActionsQuery = false;

    bool ShowIncomingNames = false;
    bool UseCircleTrans = false;
    bool StatReport = false;
    uchar CircleTransRadius = 0;
    uchar SkillReport = 0;
    ushort SpeechFont = 0;

    bool FilterPWOn = false;
    bool ObscenityFilter = false;
    string FilterPassword = "";

    bool ToggleBufficonWindow = false;
    int GameWindowX = 0;
    int GameWindowY = 0;
    int UpdateRange = g_MaxViewRange;

public:
    CConfigManager();
    ~CConfigManager() {}

    bool GetSound() { return m_Sound; };
    void SetSound(bool val);

    bool GetMusic() { return m_Music; };
    void SetMusic(bool val);

    uchar GetSoundVolume() { return m_SoundVolume; };
    void SetSoundVolume(uchar val);

    uchar GetMusicVolume() { return m_MusicVolume; };
    void SetMusicVolume(uchar val);

    uchar GetClientFPS() { return m_ClientFPS; };
    void SetClientFPS(uchar val);

    uchar GetDrawStatusState() { return m_DrawStatusState; };
    void SetDrawStatusState(uchar val);

    bool GetDrawStumps() { return m_DrawStumps; };
    void SetDrawStumps(bool val);

    bool GetMarkingCaves() { return m_MarkingCaves; };
    void SetMarkingCaves(bool val);

    bool GetNoVegetation() { return m_NoVegetation; };
    void SetNoVegetation(bool val);

    bool GetNoAnimateFields() { return m_NoAnimateFields; };
    void SetNoAnimateFields(bool val);

    bool GetConsoleNeedEnter() { return m_ConsoleNeedEnter; };
    void SetConsoleNeedEnter(bool val);

    uchar GetSpellIconAlpha() { return m_SpellIconAlpha; };
    void SetSpellIconAlpha(uchar val);

    bool GetOldStyleStatusbar() { return m_OldStyleStatusbar; };
    void SetOldStyleStatusbar(bool val);

    bool GetOriginalPartyStatusbar() { return m_OriginalPartyStatusbar; };
    void SetOriginalPartyStatusbar(bool val);

    bool GetApplyStateColorOnCharacters() { return m_ApplyStateColorOnCharacters; };
    void SetApplyStateColorOnCharacters(bool val);

    bool GetChangeFieldsGraphic() { return m_ChangeFieldsGraphic; };
    void SetChangeFieldsGraphic(bool val);

    bool GetPaperdollSlots() { return m_PaperdollSlots; };
    void SetPaperdollSlots(bool val);

    uchar GetDrawAuraState() { return m_DrawAuraState; };
    void SetDrawAuraState(uchar val);

    bool GetScaleImagesInPaperdollSlots() { return m_ScaleImagesInPaperdollSlots; };
    void SetScaleImagesInPaperdollSlots(bool val);

    bool GetUseGlobalMapLayer() { return m_UseGlobalMapLayer; };
    void SetUseGlobalMapLayer(bool val);

    bool GetNoDrawRoofs() { return m_NoDrawRoofs; };
    void SetNoDrawRoofs(bool val);

    bool GetUseGLListsForInterface() { return m_UseGLListsForInterface; };
    void SetUseGLListsForInterface(bool val);

    uchar GetPingTimer() { return m_PingTimer; };
    void SetPingTimer(uchar val);

    bool GetItemPropertiesIcon() { return m_ItemPropertiesIcon; };
    void SetItemPropertiesIcon(bool val);

    uchar GetItemPropertiesMode() { return m_ItemPropertiesMode; };
    void SetItemPropertiesMode(uchar val);

    uchar GetCharacterBackpackStyle() { return m_CharacterBackpackStyle; };
    void SetCharacterBackpackStyle(uchar val);

    void SetLightLevel(uchar val);

    void Init();

    void DefaultPage1();
    void DefaultPage2();
    void DefaultPage3();
    void DefaultPage4();

    void DefaultPage6();
    void DefaultPage7();
    void DefaultPage8();
    void DefaultPage9();

    void UpdateFeatures();

    ushort GetColorByNotoriety(uchar notoriety);

    bool Load(const string &path);

    void Save(const string &path);
};

extern CConfigManager g_ConfigManager;

extern CConfigManager g_OptionsConfig;

#endif
