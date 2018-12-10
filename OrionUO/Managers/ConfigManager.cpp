#include "stdafx.h"

#include "Dependencies/json.h"

using json = nlohmann::json;

CConfigManager g_ConfigManager;
CConfigManager g_OptionsConfig;

CConfigManager::CConfigManager()
{
    WISPFUN_DEBUG("c138_f1");
    Init();
}

void CConfigManager::Init()
{
    WISPFUN_DEBUG("c138_f2");
    DefaultPage1();

    DefaultPage2();

    DefaultPage3();

    DefaultPage4();

    DefaultPage6();

    DefaultPage7();

    DefaultPage8();

    DefaultPage9();

    g_CircleOfTransparency.Create(CircleTransRadius);

    FilterPWOn = false;
    ObscenityFilter = false;
    FilterPassword = "";

    ToggleBufficonWindow = true;

    GameWindowX = 0;
    GameWindowY = 0;

    g_MaxViewRange = MAX_VIEW_RANGE;

    UpdateRange = g_MaxViewRange;
}

void CConfigManager::DefaultPage1()
{
    WISPFUN_DEBUG("c138_f3");
    m_Sound = true;
    m_Music = true;
    FootstepsSound = true;
    CombatMusic = true;
    m_SoundVolume = 255;
    m_MusicVolume = 255;
    BackgroundSound = true;
}

void CConfigManager::DefaultPage2()
{
    WISPFUN_DEBUG("c138_f4");
    m_ClientFPS = 60;
    AutomaticallyOpenDoors = true;
    RemoveTextWithBlending = true;
    m_DrawStatusState = DCSS_NO_DRAW;
    DrawStatusConditionState = DCSCS_LOWER;
    DrawStatusConditionValue = 70;
    m_DrawStumps = false;
    m_MarkingCaves = false;
    m_NoVegetation = false;
    m_NoAnimateFields = false;
    LockGumpsMoving = true;
    m_ConsoleNeedEnter = false;
    HiddenCharactersRenderMode = 0;
    HiddenAlpha = 0x7F;
    UseHiddenModeOnlyForSelf = true;
    TransparentSpellIcons = true;
    m_SpellIconAlpha = 0x7F;
    m_OldStyleStatusbar = false;
    m_ApplyStateColorOnCharacters = false;
    m_OriginalPartyStatusbar = false;
    m_ChangeFieldsGraphic = false;
    m_PaperdollSlots = true;
    RemoveStatusbarsWithoutObjects = false;
    ShowDefaultConsoleEntryMode = true;
    m_DrawAuraState = DAS_NEVER;
    DrawAuraWithCtrlPressed = true;
    ScreenshotFormat = SF_PNG;
    m_ScaleImagesInPaperdollSlots = true;
    RemoveOrCreateObjectsWithBlending = true;
    DrawHelmetsOnShroud = false;
    m_UseGlobalMapLayer = false;
    m_NoDrawRoofs = false;
    HighlightTargetByType = true;
    AutoDisplayWorldMap = false;
    m_UseGLListsForInterface = false;
    CheckPing = true;
    m_PingTimer = 10;
    CancelNewTargetSystemOnShiftEsc = false;
    DrawStatusForHumanoids = true;
}

void CConfigManager::DefaultPage3()
{
    WISPFUN_DEBUG("c138_f5");
    UseToolTips = true;
    ToolTipsTextColor = 0xFFFF;
    ToolTipsTextFont = 0;
    ToolTipsDelay = 200;
}

void CConfigManager::DefaultPage4()
{
    WISPFUN_DEBUG("c138_f6");
    ChatColorInputText = 0xFFFF;
    ChatColorMenuOption = 0xFFFF;
    ChatColorPlayerInMemberList = 0xFFFF;
    ChatColorText = 0xFFFF;
    ChatColorPlayerNameWithout = 0xFFFF;
    ChatColorMuted = 0xFFFF;
    ChatColorChannelModeratorName = 0xFFFF;
    ChatColorChannelModeratorText = 0xFFFF;
    ChatColorMyName = 0xFFFF;
    ChatColorMyText = 0xFFFF;
    ChatColorSystemMessage = 0xFFFF;
    ChatColorBGOutputText = 0xFFFF;
    ChatColorBGInputText = 0xFFFF;
    ChatColorBGUserList = 0xFFFF;
    ChatColorBGConfList = 0xFFFF;
    ChatColorBGCommandList = 0xFFFF;

    ChatFont = 0;
}

void CConfigManager::DefaultPage6()
{
    WISPFUN_DEBUG("c138_f7");
    EnablePathfind = true;
    HoldTabForCombat = true;
    OffsetInterfaceWindows = true;
    AutoArrange = true;
    AlwaysRun = false;
    DisableMenubar = false;
    GrayOutOfRangeObjects = false;
    DisableNewTargetSystem = true;
    m_ItemPropertiesMode = OPM_FOLLOW_MOUSE;
    m_ItemPropertiesIcon = false;
    HoldShiftForContextMenus = false;
    HoldShiftForEnablePathfind = false;
    m_CharacterBackpackStyle = CBS_DEFAULT;
}

void CConfigManager::DefaultPage7()
{
    WISPFUN_DEBUG("c138_f8");
    GameWindowWidth = 800;
    GameWindowHeight = 600;
    SpeechDelay = 40;
    ScaleSpeechDelay = true;
    SpeechColor = 0x02B2;
    EmoteColor = 0x0021;
    PartyMessageColor = 0x0044;
    GuildMessageColor = 0x0044;
    AllianceMessageColor = 0x0057;
    IgnoreGuildMessage = false;
    IgnoreAllianceMessage = false;
    LightLevel = 80;
    LockResizingGameWindow = false;
}

void CConfigManager::DefaultPage8()
{
    WISPFUN_DEBUG("c138_f9");
    InnocentColor = 0x005A;
    FriendlyColor = 0x0044;
    SomeoneColor = 0x03B2;
    CriminalColor = 0x03B2;
    EnemyColor = 0x0031;
    MurdererColor = 0x0023;
    CriminalActionsQuery = true;
}

void CConfigManager::DefaultPage9()
{
    WISPFUN_DEBUG("c138_f10");
    ShowIncomingNames = true;
    UseCircleTrans = false;
    StatReport = true;
    CircleTransRadius = 64;
    SkillReport = 1;
    SpeechFont = 0;
}

void CConfigManager::UpdateFeatures()
{
    SetDrawStatusState(m_DrawStatusState);
    SetDrawStumps(m_DrawStumps);
    SetMarkingCaves(m_MarkingCaves);
    SetNoVegetation(m_NoVegetation);
    SetNoAnimateFields(m_NoAnimateFields);
    SetApplyStateColorOnCharacters(m_ApplyStateColorOnCharacters);
    SetChangeFieldsGraphic(m_ChangeFieldsGraphic);
    SetDrawAuraState(m_DrawAuraState);
    SetNoDrawRoofs(m_NoDrawRoofs);
}

void CConfigManager::SetSound(bool val)
{
    WISPFUN_DEBUG("c138_f11");

    m_Sound = val;

    if (this == &g_ConfigManager && !val)
        g_Orion.AdjustSoundEffects(g_Ticks + 100000);
}

void CConfigManager::SetMusic(bool val)
{
    WISPFUN_DEBUG("c138_f12");

    m_Music = val;

    if (this == &g_ConfigManager && !val)
    {
        g_SoundManager.StopMusic();
    }
}

void CConfigManager::SetSoundVolume(uchar val)
{
    WISPFUN_DEBUG("c138_f13");
    if (this == &g_ConfigManager && m_SoundVolume != val)
        g_Orion.AdjustSoundEffects(g_Ticks + 100000, val);

    m_SoundVolume = val;
}

void CConfigManager::SetMusicVolume(uchar val)
{
    WISPFUN_DEBUG("c138_f14");

    if (this == &g_ConfigManager && m_MusicVolume != val)
    {
        m_MusicVolume = val;
        g_SoundManager.SetMusicVolume(g_SoundManager.GetVolumeValue(-1, true));
    }
    else
        m_MusicVolume = val;
}

void CConfigManager::SetClientFPS(uchar val)
{
    WISPFUN_DEBUG("c138_f15");
    m_ClientFPS = val;

    if (this == &g_ConfigManager)
    {
        if (m_ClientFPS < MIN_FPS_LIMIT)
            m_ClientFPS = MIN_FPS_LIMIT;
        else if (m_ClientFPS > MAX_FPS_LIMIT)
            m_ClientFPS = MAX_FPS_LIMIT;

        if (m_ClientFPS == MIN_FPS_LIMIT)
        {
            g_FrameDelay = 80;
        }
        else
        {
            g_FrameDelay = 1000 / m_ClientFPS;
        }

        g_OrionWindow.SetRenderTimerDelay(g_FrameDelay);
    }
}

void CConfigManager::SetDrawStatusState(uchar val)
{
    WISPFUN_DEBUG("c138_f17");
    uchar state = val;

    if (!(g_OrionFeaturesFlags & OFF_DRAW_CHARACTERS_STATUS_IN_WORLD))
        state = DCSS_NO_DRAW;

    if (this == &g_ConfigManager)
    {
        if (state && !m_DrawStatusState)
        {
            QFOR(item, g_World->m_Items, CGameObject *)
            {
                if (item->NPC)
                    CPacketStatusRequest(item->Serial).Send();
            }
        }
    }

    m_DrawStatusState = state;
}

void CConfigManager::SetDrawStumps(bool val)
{
    WISPFUN_DEBUG("c138_f18");
    bool state = val;

    if (!(g_OrionFeaturesFlags & OFF_CHANGE_TREES_TO_STUMPS))
        state = false;

    if (m_DrawStumps != state && this == &g_ConfigManager)
        g_Orion.ClearTreesTextures();

    m_DrawStumps = state;
}

void CConfigManager::SetMarkingCaves(bool val)
{
    WISPFUN_DEBUG("c138_f19");
    bool state = val;

    if (!(g_OrionFeaturesFlags & OFF_MARKING_CAVES))
        state = false;

    if (m_MarkingCaves != state && this == &g_ConfigManager)
        g_Orion.ClearCaveTextures();

    m_MarkingCaves = state;
}

void CConfigManager::SetNoVegetation(bool val)
{
    WISPFUN_DEBUG("c138_f19");
    bool state = val;

    if (!(g_OrionFeaturesFlags & OFF_NO_VEGETATION))
        state = false;

    m_NoVegetation = state;
}

void CConfigManager::SetNoAnimateFields(bool val)
{
    WISPFUN_DEBUG("c138_f19");
    bool state = val;

    if (!(g_OrionFeaturesFlags & OFF_NO_FIELDS_ANIMATION))
        state = false;

    m_NoAnimateFields = state;
}

void CConfigManager::SetApplyStateColorOnCharacters(bool val)
{
    WISPFUN_DEBUG("c138_f19");
    bool state = val;

    if (!(g_OrionFeaturesFlags & OFF_COLORED_CHARACTERS_STATE))
        state = false;

    m_ApplyStateColorOnCharacters = state;
}

void CConfigManager::SetDrawAuraState(uchar val)
{
    WISPFUN_DEBUG("c138_f19");
    uchar state = val;

    if (!(g_OrionFeaturesFlags & OFF_DRAW_AURA))
        state = DAS_NEVER;

    m_DrawAuraState = state;
}

void CConfigManager::SetConsoleNeedEnter(bool val)
{
    WISPFUN_DEBUG("c138_f21");
    if (this == &g_ConfigManager && val && g_EntryPointer == &g_GameConsole)
        g_EntryPointer = NULL;

    m_ConsoleNeedEnter = val;
}

void CConfigManager::SetSpellIconAlpha(uchar val)
{
    WISPFUN_DEBUG("c138_f22");
    if (this == &g_ConfigManager && val != m_SpellIconAlpha)
    {
        float alpha = val / 255.0f;
        bool redraw = g_ConfigManager.TransparentSpellIcons;

        QFOR(gump, g_GumpManager.m_Items, CGump *)
        {
            if (gump->GumpType == GT_SPELL)
            {
                ((CGumpSpell *)gump)->m_Blender->Alpha = alpha;

                if (redraw)
                    gump->WantRedraw = true;
            }
        }
    }

    m_SpellIconAlpha = val;
}

void CConfigManager::SetOldStyleStatusbar(bool val)
{
    WISPFUN_DEBUG("c138_f23");

    m_OldStyleStatusbar = val;

    if (this == &g_ConfigManager)
    {
        CGump *gump = g_GumpManager.UpdateGump(g_PlayerSerial, 0, GT_STATUSBAR);

        if (gump != NULL && !gump->Minimized)
            gump->WantUpdateContent = true;
    }
}

void CConfigManager::SetOriginalPartyStatusbar(bool val)
{
    WISPFUN_DEBUG("c138_f24");
    m_OriginalPartyStatusbar = val;

    if (this == &g_ConfigManager)
    {
        if (g_Party.Leader != 0)
        {
            QFOR(gump, g_GumpManager.m_Items, CGump *)
            {
                if (gump->GumpType == GT_STATUSBAR &&
                    (g_Party.Leader == gump->Serial || g_Party.Contains(gump->Serial)))
                {
                    gump->WantRedraw = true;
                    gump->WantUpdateContent = true;
                }
            }
        }
    }
}

void CConfigManager::SetChangeFieldsGraphic(bool val)
{
    WISPFUN_DEBUG("c138_f25");
    m_ChangeFieldsGraphic = val;

    if (!(g_OrionFeaturesFlags & OFF_TILED_FIELDS))
        m_ChangeFieldsGraphic = false;

    if (this == &g_ConfigManager && g_World != NULL)
    {
        QFOR(item, g_World->m_Items, CGameObject *)
        {
            if (!item->NPC)
                ((CGameItem *)item)->CalculateFieldColor();
        }
    }
}

void CConfigManager::SetPaperdollSlots(bool val)
{
    WISPFUN_DEBUG("c138_f26");

    m_PaperdollSlots = val;

    if (this == &g_ConfigManager && g_World != NULL)
    {
        QFOR(gump, g_GumpManager.m_Items, CGump *)
        {
            if (gump->GumpType == GT_PAPERDOLL)
            {
                gump->WantRedraw = true;
                gump->WantUpdateContent = true;
            }
        }
    }
}

void CConfigManager::SetScaleImagesInPaperdollSlots(bool val)
{
    WISPFUN_DEBUG("c138_f27");

    m_ScaleImagesInPaperdollSlots = val;

    if (this == &g_ConfigManager && g_World != NULL)
    {
        QFOR(gump, g_GumpManager.m_Items, CGump *)
        {
            if (gump->GumpType == GT_PAPERDOLL)
            {
                gump->WantRedraw = true;
                gump->WantUpdateContent = true;
            }
        }
    }
}

void CConfigManager::SetUseGlobalMapLayer(bool val)
{
    WISPFUN_DEBUG("c138_f28");

    m_UseGlobalMapLayer = val;

    if (this == &g_ConfigManager && val)
    {
        QFOR(gump, g_GumpManager.m_Items, CGump *)
        {
            if (gump->GumpType == GT_WORLD_MAP)
                g_GumpManager.MoveToFront(gump);
        }
    }
}

void CConfigManager::SetNoDrawRoofs(bool val)
{
    WISPFUN_DEBUG("c138_f28");
    m_NoDrawRoofs = val;

    if (!(g_OrionFeaturesFlags & OFF_TILED_FIELDS))
        m_NoDrawRoofs = false;

    if (this == &g_ConfigManager && g_Player != NULL)
    {
        g_GameScreen.OldX = 0;
        g_GameScreen.OldY = 0;
    }
}

void CConfigManager::SetUseGLListsForInterface(bool val)
{
    WISPFUN_DEBUG("c138_f26");

    bool old = m_UseGLListsForInterface;
    m_UseGLListsForInterface = (val || !g_GL.CanUseFrameBuffer);

    if (this == &g_ConfigManager && g_World != NULL && old != m_UseGLListsForInterface)
    {
        QFOR(gump, g_GumpManager.m_Items, CGump *)
        {
            gump->WantRedraw = true;
            gump->WantUpdateContent = true;
        }
    }
}

void CConfigManager::SetPingTimer(uchar val)
{
    WISPFUN_DEBUG("c138_f26");

    m_PingTimer = max(min(val, 120), 10);
    g_PingTimer = 0;
}

void CConfigManager::SetItemPropertiesMode(uchar val)
{
    WISPFUN_DEBUG("c138_f26_1");

    m_ItemPropertiesMode = val;

    if (this == &g_ConfigManager && g_World != NULL)
    {
        CGumpPropertyIcon *gump =
            (CGumpPropertyIcon *)g_GumpManager.UpdateContent(0, 0, GT_PROPERTY_ICON);

        if (gump != NULL && (val == OPM_AT_ICON || val == OPM_ALWAYS_UP))
            gump->SetText(gump->GetText());

        g_ObjectPropertiesManager.Reset();
    }
}

void CConfigManager::SetItemPropertiesIcon(bool val)
{
    WISPFUN_DEBUG("c138_f26_2");

    m_ItemPropertiesIcon = val;

    if (this == &g_ConfigManager && g_World != NULL)
    {
        if (val)
        {
            CGump *gump = g_GumpManager.UpdateContent(0, 0, GT_PROPERTY_ICON);

            if (gump == NULL)
            {
                WISP_GEOMETRY::CSize windowSize = g_OrionWindow.GetSize();

                int x = GameWindowX + (int)(GameWindowWidth * 0.9f);
                int y = GameWindowY + GameWindowHeight;

                if (x + 100 >= windowSize.Width)
                    x = windowSize.Width - 100;

                if (y + 60 >= windowSize.Height)
                    y = windowSize.Height - 60;

                g_GumpManager.AddGump(new CGumpPropertyIcon(x, y));
            }
        }
        else
        {
            g_GumpManager.CloseGump(0, 0, GT_PROPERTY_ICON);
            g_ObjectPropertiesManager.Reset();
        }
    }
}

void CConfigManager::SetCharacterBackpackStyle(uchar val)
{
    WISPFUN_DEBUG("c138_f26_3");

    m_CharacterBackpackStyle = val;

    if (this == &g_ConfigManager && g_World != NULL)
    {
        g_GumpManager.UpdateContent(g_PlayerSerial, 0, GT_PAPERDOLL);

        CGameItem *backpack = g_Player->FindLayer(OL_BACKPACK);

        if (backpack != NULL)
            g_GumpManager.UpdateContent(backpack->Serial, 0, GT_CONTAINER);
    }
}

void CConfigManager::SetLightLevel(uchar val)
{
    LightLevel = val;

    g_GameScreen.CalculateLightLevel();
}

ushort CConfigManager::GetColorByNotoriety(uchar notoriety)
{
    WISPFUN_DEBUG("c138_f29");
    ushort color = 0;

    switch ((NOTORIETY_TYPE)notoriety)
    {
        case NT_INNOCENT:
        {
            color = InnocentColor;
            break;
        }
        case NT_FRIENDLY:
        {
            color = FriendlyColor;
            break;
        }
        case NT_SOMEONE_GRAY:
        {
            color = SomeoneColor;
            break;
        }
        case NT_CRIMINAL:
        {
            color = CriminalColor;
            break;
        }
        case NT_ENEMY:
        {
            color = EnemyColor;
            break;
        }
        case NT_MURDERER:
        {
            color = MurdererColor;
            break;
        }
        case NT_INVULNERABLE:
        {
            color = 0x0034;
            break;
        }
        default:
            break;
    }

    return color;
}

bool CConfigManager::Load(const string &path)
{
    std::ifstream i;

    bool zoomed = false;
    int windowX = -1;
    int windowY = -1;
    int windowWidth = -1;
    int windowHeight = -1;

    try
    {
        json j;
        i.open(path);
        i >> j;

        UpdateRange = g_MaxViewRange;

        j.at("Sound");

        SetSound(j["Sound"]);
        m_SoundVolume = j["Soundvolume"];
        SetMusic(j["Music"]);
        m_MusicVolume = j["MusicVolume"];
        FootstepsSound = j["FootstepsSound"];
        CombatMusic = j["CombatMusic"];
        BackgroundSound = j["BackgroundSound"];

        SetClientFPS(j["ClientFPS"]);
        AutomaticallyOpenDoors = j["AutomaticallyOpenDoors"];
        RemoveTextWithBlending = j["RemoveTextWithBlending"];
        m_DrawStatusState = j["DrawStatusState"];
        SetDrawStumps(j["DrawStumps"]);
        SetMarkingCaves(j["MarkingCaves"]);
        SetNoAnimateFields(j["NoAnimateFields"]);
        SetNoVegetation(j["NoVegetation"]);
        HiddenCharactersRenderMode = j["HiddenCharactersRenderMode"];
        HiddenAlpha = j["HiddenAlpha"];
        UseHiddenModeOnlyForSelf = j["UseHiddenModeOnlyForSelf"];
        TransparentSpellIcons = j["TransparentSpellIcons"];
        m_SpellIconAlpha = j["SpellIconAlpha"];
        m_OldStyleStatusbar = j["OldStyleStatusbar"];
        m_OriginalPartyStatusbar = j["OriginalPartyStatusbar"];
        SetApplyStateColorOnCharacters(j["ApplyStateColorOnCharacters"]);
        SetChangeFieldsGraphic(j["ChangeFieldsGraphic"]);
        SetPaperdollSlots(j["PaperdollSlots"]);
        DrawStatusConditionState = j["DrawStatusConditionState"];
        DrawStatusConditionValue = j["DrawStatusConditionValue"];
        RemoveStatusbarsWithoutObjects = j["RemoveStatusbarsWithoutObjects"];
        ShowDefaultConsoleEntryMode = j["ShowDefaultConsoleEntryMode"];
        SetDrawAuraState(j["DrawAuraState"]);
        DrawAuraWithCtrlPressed = j["DrawAuraWithCtrlPressed"];
        ScreenshotFormat = j["ScreenshotFormat"];
        SetScaleImagesInPaperdollSlots(j["ScaleImagesInPaperdollSlots"]);
        RemoveOrCreateObjectsWithBlending = j["RemoveOrCreateObjectsWithBlending"];
        DrawHelmetsOnShroud = j["DrawHelmetsOnShroud"];
        SetUseGlobalMapLayer(j["UseGlobalMapLayer"]);
        SetNoDrawRoofs(j["NoDrawRoofs"]);
        HighlightTargetByType = j["HighlightTargetByType"];
        AutoDisplayWorldMap = j["AutoDisplayWorldMap"];
        SetUseGLListsForInterface(j["UseGLListsForInterface"]);
        CheckPing = j["CheckPing"];
        SetPingTimer(j["PingTimer"]);
        CancelNewTargetSystemOnShiftEsc = j["CancelNewTargetSystemOnShiftEsc"];
        DrawStatusForHumanoids = j["DrawStatusForHumanoids"];

        UseToolTips = j["UseToolTips"];
        ToolTipsTextColor = j["ToolTipsTextColor"];
        ToolTipsTextFont = j["ToolTipsTextFont"];
        ToolTipsDelay = j["ToolTipsDelay"];

        ChatColorInputText = j["ChatColorInputText"];
        ChatColorMenuOption = j["ChatColorMenuOption"];
        ChatColorPlayerInMemberList = j["ChatColorPlayerInMemberList"];
        ChatColorText = j["ChatColorText"];
        ChatColorPlayerNameWithout = j["ChatColorPlayerNameWithout"];
        ChatColorMuted = j["ChatColorMuted"];
        ChatColorChannelModeratorName = j["ChatColorChannelModeratorName"];
        ChatColorChannelModeratorText = j["ChatColorChannelModeratorText"];
        ChatColorMyName = j["ChatColorMyName"];
        ChatColorMyText = j["ChatColorMyText"];
        ChatColorSystemMessage = j["ChatColorSystemMessage"];
        ChatFont = j["ChatFont"];
        ChatColorBGOutputText = j["ChatColorBGOutputText"];
        ChatColorBGInputText = j["ChatColorBGInputText"];
        ChatColorBGUserList = j["ChatColorBGUserList"];
        ChatColorBGConfList = j["ChatColorBGConfList"];
        ChatColorBGCommandList = j["ChatColorBGCommandList"];

        EnablePathfind = j["EnablePathfind"];
        HoldTabForCombat = j["HoldTabForCombat"];
        OffsetInterfaceWindows = j["OffsetInterfaceWindows"];
        AutoArrange = j["AutoArrange"];
        AlwaysRun = j["AlwaysRun"];
        DisableMenubar = j["DisableMenubar"];
        GrayOutOfRangeObjects = j["GrayOutOfRangeObjects"];
        DisableNewTargetSystem = j["DisableNewTargetSystem"];
        m_ItemPropertiesMode = j["ItemPropertiesMode"];
        m_ItemPropertiesIcon = j["ItemPropertiesIcon"];
        HoldShiftForContextMenus = j["HoldShiftForContextMenus"];
        HoldShiftForEnablePathfind = j["HoldShiftForEnablePathfind"];
        g_ContainerRect.DefaultX = j["ContainerDefaultX"];
        g_ContainerRect.DefaultY = j["ContainerDefaultY"];
        m_CharacterBackpackStyle = j["CharacterBackpackStyle"];

        GameWindowWidth = j["GameWindowWidth"];
        GameWindowHeight = j["GameWindowHeight"];
        SpeechDelay = j["SpeechDelay"];
        ScaleSpeechDelay = j["ScaleSpeechDelay"];
        SpeechColor = j["SpeechColor"];
        EmoteColor = j["EmoteColor"];
        PartyMessageColor = j["PartyMessageColor"];
        GuildMessageColor = j["GuildMessageColor"];
        AllianceMessageColor = j["AllianceMessageColor"];
        IgnoreGuildMessage = j["IgnoreGuildMessage"];
        IgnoreAllianceMessage = j["IgnoreAllianceMessage"];
        SetLightLevel(j["LightLevel"]);
        LockResizingGameWindow = j["LockResizingGameWindow"];
        LockGumpsMoving = j["LockGumpsMoving"];

        InnocentColor = j["InnocentColor"];
        FriendlyColor = j["FriendlyColor"];
        SomeoneColor = j["SomeoneColor"];
        CriminalColor = j["CriminalColor"];
        EnemyColor = j["EnemyColor"];
        MurdererColor = j["MurdererColor"];
        CriminalActionsQuery = j["CriminalActionsQuery"];

        ShowIncomingNames = j["ShowIncomingNames"];
        UseCircleTrans = j["UseCircleTrans"];
        StatReport = j["StatReport"];
        m_ConsoleNeedEnter = j["ConsoleNeedEnter"];
        g_CircleOfTransparency.Create(j["CircleTransRadius"]);
        SkillReport = j["SkillReport"];
        SpeechFont = j["SpeechFont"];

        GameWindowX = j["GameWindowX"];
        GameWindowY = j["GameWindowY"];

        zoomed = j["Zoomed"];
        windowX = j["RealX"];
        windowY = j["RealY"];
        windowWidth = j["RealWidth"];
        windowHeight = j["RealHeight"];

        ToggleBufficonWindow = j["ToggleBufficonWindow"];
        g_DeveloperMode = j["DeveloperMode"];

        if (g_World == NULL)
            g_ServerList.LastServerName = j["LastServer"].get<std::string>();

        if (g_World == NULL)
            g_CharacterList.LastCharacterName = j["LastCharacter"].get<std::string>();
    }
    catch (std::ios_base::failure &fail)
    {
        return false;
    }
    catch (nlohmann::detail::parse_error &fail)
    {
        return false;
    }

    if (SpeechDelay > 100)
    {
        SpeechDelay = 40;
    }

    if (GameWindowX < 0)
        GameWindowX = 0;

    if (GameWindowY < 0)
        GameWindowY = 0;

    if (windowX != -1 && windowY != -1 && windowWidth != -1 && windowHeight != -1)
    {
        if (windowX < 0)
            windowX = 0;

        if (windowY < 0)
            windowY = 0;

        if (g_GameState >= GS_GAME)
        {
            if (windowWidth < 640)
                windowWidth = 640;

            if (windowWidth >= GetSystemMetrics(SM_CXSCREEN) - 20)
                windowWidth = GetSystemMetrics(SM_CXSCREEN) - 20;

            if (windowHeight < 480)
                windowHeight = 480;

            if (windowHeight >= GetSystemMetrics(SM_CYSCREEN) - 60)
                windowHeight = GetSystemMetrics(SM_CYSCREEN) - 60;
        }
        else
        {
            windowWidth = 640;
            windowHeight = 480;
        }

        SendMessage(g_OrionWindow.Handle, WM_SYSCOMMAND, SC_RESTORE, 0);

        if (zoomed)
            SendMessage(g_OrionWindow.Handle, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
        else
            SetWindowPos(
                g_OrionWindow.Handle, NULL, windowX, windowY, windowWidth, windowHeight, 0);

        g_GL.UpdateRect();
    }
    else
    {
        SendMessage(g_OrionWindow.Handle, WM_SYSCOMMAND, SC_RESTORE, 0);
        SendMessage(g_OrionWindow.Handle, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
    }

    return PathFileExistsA(path.c_str());
}

void CConfigManager::Save(const string &path)
{
    WISPFUN_DEBUG("");

    json j = json::object();

    j["Sound"] = m_Sound;
    j["Soundvolume"] = m_SoundVolume;
    j["Music"] = m_Music;
    j["MusicVolume"] = m_MusicVolume;
    j["FootstepsSound"] = FootstepsSound;
    j["CombatMusic"] = CombatMusic;
    j["BackgroundSound"] = BackgroundSound;

    j["ClientFPS"] = m_ClientFPS;
    j["AutomaticallyOpenDoors"] = AutomaticallyOpenDoors;
    j["RemoveTextWithBlending"] = RemoveTextWithBlending;
    j["DrawStatusState"] = m_DrawStatusState;
    j["DrawStumps"] = m_DrawStumps;
    j["MarkingCaves"] = m_MarkingCaves;
    j["NoAnimateFields"] = m_NoAnimateFields;
    j["NoVegetation"] = m_NoVegetation;
    j["HiddenCharactersRenderMode"] = HiddenCharactersRenderMode;
    j["HiddenAlpha"] = HiddenAlpha;
    j["UseHiddenModeOnlyForSelf"] = UseHiddenModeOnlyForSelf;
    j["TransparentSpellIcons"] = TransparentSpellIcons;
    j["SpellIconAlpha"] = m_SpellIconAlpha;
    j["OldStyleStatusbar"] = m_OldStyleStatusbar;
    j["OriginalPartyStatusbar"] = m_OriginalPartyStatusbar;
    j["ApplyStateColorOnCharacters"] = m_ApplyStateColorOnCharacters;
    j["ChangeFieldsGraphic"] = m_ChangeFieldsGraphic;
    j["PaperdollSlots"] = m_PaperdollSlots;
    j["DrawStatusConditionState"] = DrawStatusConditionState;
    j["DrawStatusConditionValue"] = DrawStatusConditionValue;
    j["RemoveStatusbarsWithoutObjects"] = RemoveStatusbarsWithoutObjects;
    j["ShowDefaultConsoleEntryMode"] = ShowDefaultConsoleEntryMode;
    j["DrawAuraState"] = m_DrawAuraState;
    j["DrawAuraWithCtrlPressed"] = DrawAuraWithCtrlPressed;
    j["ScreenshotFormat"] = ScreenshotFormat;
    j["ScaleImagesInPaperdollSlots"] = m_ScaleImagesInPaperdollSlots;
    j["RemoveOrCreateObjectsWithBlending"] = RemoveOrCreateObjectsWithBlending;
    j["DrawHelmetsOnShroud"] = DrawHelmetsOnShroud;
    j["UseGlobalMapLayer"] = m_UseGlobalMapLayer;
    j["NoDrawRoofs"] = m_NoDrawRoofs;
    j["HighlightTargetByType"] = HighlightTargetByType;
    j["AutoDisplayWorldMap"] = AutoDisplayWorldMap;
    j["UseGLListsForInterface"] = m_UseGLListsForInterface;
    j["CheckPing"] = CheckPing;
    j["PingTimer"] = m_PingTimer;
    j["CancelNewTargetSystemOnShiftEsc"] = CancelNewTargetSystemOnShiftEsc;
    j["DrawStatusForHumanoids"] = DrawStatusForHumanoids;

    j["UseToolTips"] = UseToolTips;
    j["ToolTipsTextColor"] = ToolTipsTextColor;
    j["ToolTipsTextFont"] = ToolTipsTextFont;
    j["ToolTipsDelay"] = ToolTipsDelay;

    j["ChatColorInputText"] = ChatColorInputText;
    j["ChatColorMenuOption"] = ChatColorMenuOption;
    j["ChatColorPlayerInMemberList"] = ChatColorPlayerInMemberList;
    j["ChatColorText"] = ChatColorText;
    j["ChatColorPlayerNameWithout"] = ChatColorPlayerNameWithout;
    j["ChatColorMuted"] = ChatColorMuted;
    j["ChatColorChannelModeratorName"] = ChatColorChannelModeratorName;
    j["ChatColorChannelModeratorText"] = ChatColorChannelModeratorText;
    j["ChatColorMyName"] = ChatColorMyName;
    j["ChatColorMyText"] = ChatColorMyText;
    j["ChatColorSystemMessage"] = ChatColorSystemMessage;
    j["ChatFont"] = ChatFont;
    j["ChatColorBGOutputText"] = ChatColorBGOutputText;
    j["ChatColorBGInputText"] = ChatColorBGInputText;
    j["ChatColorBGUserList"] = ChatColorBGUserList;
    j["ChatColorBGConfList"] = ChatColorBGConfList;
    j["ChatColorBGCommandList"] = ChatColorBGCommandList;

    j["EnablePathfind"] = EnablePathfind;
    j["HoldTabForCombat"] = HoldTabForCombat;
    j["OffsetInterfaceWindows"] = OffsetInterfaceWindows;
    j["AutoArrange"] = AutoArrange;
    j["AlwaysRun"] = AlwaysRun;
    j["DisableMenubar"] = DisableMenubar;
    j["GrayOutOfRangeObjects"] = GrayOutOfRangeObjects;
    j["DisableNewTargetSystem"] = DisableNewTargetSystem;
    j["ItemPropertiesMode"] = m_ItemPropertiesMode;
    j["ItemPropertiesIcon"] = m_ItemPropertiesIcon;
    j["HoldShiftForContextMenus"] = HoldShiftForContextMenus;
    j["HoldShiftForEnablePathfind"] = HoldShiftForEnablePathfind;
    j["ContainerDefaultX"] = g_ContainerRect.DefaultX;
    j["ContainerDefaultY"] = g_ContainerRect.DefaultY;
    j["CharacterBackpackStyle"] = GetCharacterBackpackStyle();

    j["GameWindowWidth"] = GameWindowWidth;
    j["GameWindowHeight"] = GameWindowHeight;
    j["SpeechDelay"] = SpeechDelay;
    j["ScaleSpeechDelay"] = ScaleSpeechDelay;
    j["SpeechColor"] = SpeechColor;
    j["EmoteColor"] = EmoteColor;
    j["PartyMessageColor"] = PartyMessageColor;
    j["GuildMessageColor"] = GuildMessageColor;
    j["AllianceMessageColor"] = AllianceMessageColor;
    j["IgnoreGuildMessage"] = IgnoreGuildMessage;
    j["IgnoreAllianceMessage"] = IgnoreAllianceMessage;
    j["LightLevel"] = LightLevel;
    j["LockResizingGameWindow"] = LockResizingGameWindow;
    j["LockGumpsMoving"] = LockGumpsMoving;

    j["InnocentColor"] = InnocentColor;
    j["FriendlyColor"] = FriendlyColor;
    j["SomeoneColor"] = SomeoneColor;
    j["CriminalColor"] = CriminalColor;
    j["EnemyColor"] = EnemyColor;
    j["MurdererColor"] = MurdererColor;
    j["CriminalActionsQuery"] = CriminalActionsQuery;

    j["ShowIncomingNames"] = ShowIncomingNames;
    j["UseCircleTrans"] = UseCircleTrans;
    j["StatReport"] = StatReport;
    j["ConsoleNeedEnter"] = m_ConsoleNeedEnter;
    j["CircleTransRadius"] = CircleTransRadius;
    j["SkillReport"] = SkillReport;
    j["SpeechFont"] = SpeechFont;

    j["GameWindowX"] = GameWindowX;
    j["GameWindowY"] = GameWindowY;

    j["Zoomed"] = g_OrionWindow.Zoomed();

    RECT rect = { 0 };
    GetWindowRect(g_OrionWindow.Handle, &rect);

    j["RealX"] = rect.left;
    j["RealY"] = rect.top;
    j["RealWidth"] = (rect.right - rect.left);
    j["RealHeight"] = (rect.bottom - rect.top);

    j["ToggleBufficonWindow"] = ToggleBufficonWindow;
    j["DeveloperMode"] = g_DeveloperMode;

    j["LastServer"] = g_ServerList.LastServerName;
    j["LastCharacter"] = g_CharacterList.LastCharacterName;

    std::ofstream o(path);
    o << std::setw(4) << j << std::endl;
}
