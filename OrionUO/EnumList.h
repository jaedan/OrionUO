

#ifndef ENUMLIST_H
#define ENUMLIST_H

enum GAME_STATE
{
    GS_MAIN = 0,
    GS_MAIN_CONNECT,
    GS_SERVER,
    GS_SERVER_CONNECT,
    GS_CHARACTER,
    GS_DELETE,
    GS_PROFESSION_SELECT,
    GS_CREATE,
    GS_GAME_CONNECT,
    GS_GAME,
    GS_GAME_BLOCKED

};

enum RENDER_OBJECT_TYPE
{
    ROT_NO_OBJECT = 0,
    ROT_GAME_OBJECT,
    ROT_STATIC_OBJECT,
    ROT_LAND_OBJECT,
    ROT_MULTI_OBJECT,
    ROT_EFFECT
};

enum SCAN_TYPE_OBJECT
{
    STO_HOSTILE = 0,
    STO_PARTY,
    STO_FOLLOWERS,
    STO_OBJECTS,
    STO_MOBILES
};

enum SCAN_MODE_OBJECT
{
    SMO_NEXT = 0,
    SMO_PREV,
    SMO_NEAREST
};

enum EFFECT_TYPE
{
    EF_MOVING = 0,
    EF_LIGHTING,
    EF_STAY_AT_POS,
    EF_STAY_AT_SOURCE,
    EF_DRAG
};

enum CHARACTER_SPEED_TYPE
{
    CST_NORMAL = 0,
    CST_FAST_UNMOUNT,
    CST_CANT_RUN,
    CST_FAST_UNMOUNT_AND_CANT_RUN
};

enum ENCRYPTION_TYPE
{
    ET_NOCRYPT = 0,
    ET_OLD_BFISH,
    ET_1_25_36,
    ET_BFISH,
    ET_203,
    ET_TFISH
};

enum CONNECTION_SCREEN_TYPE
{
    CST_LOGIN = 0,
    CST_CHARACTER_LIST,
    CST_CONLOST,
    CST_SELECT_PROFESSOIN,
    CST_GAME_LOGIN,
    CST_GAME
};

enum GENDER
{
    GENDER_MALE = 0,
    GENDER_FEMALE
};

enum PROFESSION_TYPE
{
    PT_NO_PROF = 0,
    PT_CATEGORY,
    PT_PROFESSION
};

enum CLIENT_FLAG
{
    CF_T2A = 0x00,
    CF_RE = 0x01,
    CF_TD = 0x02,
    CF_LBR = 0x04,
    CF_AOS = 0x08,
    CF_SE = 0x10,
    CF_SA = 0x20,
    CF_UO3D = 0x40,
    CF_RESERVED = 0x80,
    CF_3D = 0x100
};

enum CHARACTER_LIST_FLAG
{
    CLF_UNKNOWN = 0x01,
    CLF_OWERWRITE_CONFIGURATION_BUTTON = 0x02,
    CLF_ONE_CHARACTER_SLOT = 0x04,
    CLF_CONTEXT_MENU = 0x08,
    CLF_LIMIT_CHARACTER_SLOTS = 0x10,
    CLF_PALADIN_NECROMANCER_TOOLTIPS = 0x20,
    CLF_6_CHARACTER_SLOT = 0x40,
    CLF_SAMURAI_NINJA = 0x80,
    CLF_ELVEN_RACE = 0x100,
    CLF_UNKNOWN_1 = 0x200,
    CLF_UO3D = 0x400,
    CLF_UNKNOWN_2 = 0x800,
    CLF_7_CHARACTER_SLOT = 0x1000,
    CLF_UNKNOWN_3 = 0x2000,
    CLF_NEW_MOVEMENT_SYSTEM = 0x4000,
    CLF_UNLOCK_FELUCCA_AREAS = 0x8000
};

enum LOCKED_FEATURE_FLAG
{
    LFF_T2A = 0x01,
    LFF_RE = 0x02,
    LFF_TD = 0x04,
    LFF_LBR = 0x08,
    LFF_AOS = 0x10,
    LFF_6_SLOT = 0x20,
    LFF_SE = 0x40,
    LFF_ML = 0x80,
    LFF_8_AGE = 0x100,
    LFF_9_AGE = 0x200,
    LFF_10_AGE = 0x400,
    LFF_HOUSING = 0x800,
    LFF_7_SLOT = 0x1000,
    LFF_KR = 0x2000,
    LFF_TRIAL_ACC = 0x4000,
    LFF_11_AGE = 0x8000,
    LFF_SA = 0x10000,
    LFF_HSA = 0x20000,
    LFF_GOTHIC_HOUSING = 0x40000,
    LFF_RUSTIC_HOUSING = 0x80000
};

enum SCREEN_EFFECT_MODE
{
    SEM_NONE = 0,
    SEM_SUNRISE,
    SEM_SUNSET
};

enum SCREEN_EFFECT_TYPE
{
    SET_TO_BLACK,
    SET_TO_WHITE,
    SET_TO_WHITE_FAST,
    SET_TO_WHITE_THEN_BLACK,
    SET_TO_BLACK_VERY_FAST
};

enum WEATHER_TYPE
{
    WT_RAIN = 0,
    WT_FIERCE_STORM,
    WT_SNOW,
    WT_STORM
};

enum GUMP_TYPE
{
    GT_BLANK = 0,
    GT_PAPERDOLL,
    GT_STATUSBAR,
    GT_DYE,
    GT_OPTIONS,
    GT_GENERIC,
    GT_MENU,
    GT_GRAY_MENU,
    GT_TRADE,
    GT_SHOP,
    GT_MINIMAP,
    GT_MAP,
    GT_SKILLS,
    GT_JOURNAL,
    GT_CONTAINER,
    GT_SPELLBOOK,
    GT_SPELL,
    GT_DRAG,
    GT_TEXT_ENTRY_DIALOG,
    GT_QUESTION,
    GT_SELECT_COLOR,
    GT_SELECT_FONT,
    GT_NOTIFY,
    GT_BOOK,
    GT_TIP,
    GT_PARTY_MANIFEST,
    GT_MAIL,
    GT_PROFILE,
    GT_MENUBAR,
    GT_BUFF,
    GT_CHAT,
    GT_BULLETIN_BOARD,
    GT_BULLETIN_BOARD_ITEM,
    GT_WORLD_MAP,
    GT_PLUGIN,
    GT_TARGET_SYSTEM,
    GT_POPUP_MENU,
    GT_CONSOLE_TYPE,
    GT_SKILL,
    GT_COMBAT_BOOK,
    GT_ABILITY,
    GT_CUSTOM_HOUSE,
    GT_PROPERTY,
    GT_PROPERTY_ICON
};

enum GUMP_OBJECT_TYPE
{
    GOT_NONE = 0,
    GOT_PAGE,
    GOT_GROUP,
    GOT_MASTERGUMP,
    GOT_RESIZEPIC,
    GOT_SCOPE,
    GOT_CHECKTRANS,
    GOT_SCISSOR,
    GOT_COLOREDPOLYGON,
    GOT_LINE,
    GOT_SLIDER,
    GOT_MINMAXBUTTONS,
    GOT_SHADER,
    GOT_HITBOX,
    GOT_DATABOX,
    GOT_BUTTON,
    GOT_RESIZEBUTTON,
    GOT_BUTTONTILEART,
    GOT_BLENDING,
    GOT_GLOBAL_COLOR,
    GOT_BUFF,
    GOT_SHOPITEM,
    GOT_SHOPRESULT,
    GOT_SKILLITEM,
    GOT_SKILLGROUP,
    GOT_BB_OBJECT,
    GOT_SCROLLBACKGROUND,
    GOT_MENUOBJECT,
    GOT_COMBOBOX,
    GOT_CHECKBOX,
    GOT_RADIO,
    GOT_EXTERNALTEXTURE,
    GOT_TILEPIC,
    GOT_TILEPICHIGHTLIGHTED,
    GOT_GUMPPIC,
    GOT_GUMPPICTILED,
    GOT_GUMPPICHIGHTLIGHTED,
    GOT_TEXT,
    GOT_HTMLTEXT,
    GOT_TEXTENTRY,
    GOT_HTMLGUMP,
    GOT_XFMHTMLGUMP,
    GOT_XFMHTMLTOKEN,
    GOT_TOOLTIP,
    GOT_VIRTURE_GUMP
};

enum SLIDER_TEXT_POSITION
{
    STP_TOP = 0,
    STP_BOTTOM,
    STP_LEFT,
    STP_RIGHT,
    STP_TOP_CENTER,
    STP_BOTTOM_CENTER,
    STP_LEFT_CENTER,
    STP_RIGHT_CENTER
};

enum SELECT_COLOR_GUMP_STATE
{

    SCGS_OPT_TOOLTIP_TEXT = 0,
    SCGS_OPT_CHAT_INPUT_TEXT,
    SCGS_OPT_CHAT_MENU_OPTION,
    SCGS_OPT_CHAT_PLAYER_IN_MEMBER_LIST,
    SCGS_OPT_CHAT_TEXT,
    SCGS_OPT_CHAT_PLAYER_WITHOUT_PRIV,
    SCGS_OPT_CHAT_MUTED_TEXT,
    SCGS_OPT_CHAT_CHAN_MODER_NAME,
    SCGS_OPT_CHAT_CHAN_MODER_TEXT,
    SCGS_OPT_CHAT_MY_NAME,
    SCGS_OPT_CHAT_MY_TEXT,
    SCGS_OPT_CHAT_SYSTEM_MESSAGE,
    SCGS_OPT_CHAT_BG_OUTPUT,
    SCGS_OPT_CHAT_BG_INPUT,
    SCGS_OPT_CHAT_BG_USER_LIST,
    SCGS_OPT_CHAT_BG_CONF_LIST,
    SCGS_OPT_CHAT_BG_COMMAND_LIST,
    SCGS_OPT_DISPLAY_SPEECH,
    SCGS_OPT_DISPLAY_EMOTE,
    SCGS_OPT_DISPLAY_PARTY_MESSAGE,
    SCGS_OPT_DISPLAY_GUILD_MESSAGE,
    SCGS_OPT_DISPLAY_ALLIANCE_MESSAGE,
    SCGS_OPT_REPSYS_INNOCENT,
    SCGS_OPT_REPSYS_FRIENDLY,
    SCGS_OPT_REPSYS_SOMEONE,
    SCGS_OPT_REPSYS_CRIMINAL,
    SCGS_OPT_REPSYS_ENEMY,
    SCGS_OPT_REPSYS_MURDERER
};

enum SELECT_FONT_GUMP_STATE
{
    SFGS_OPT_POPUP = 1,
    SFGS_OPT_TOOLTIP,
    SFGS_OPT_CHAT,
    SFGS_OPT_MISCELLANEOUS
};

enum MAP_MESSAGE
{
    MM_ADD = 1,
    MM_INSERT,
    MM_MOVE,
    MM_REMOVE,
    MM_CLEAR,
    MM_EDIT,
    MM_EDIT_RESPONSE
};

enum ORION_COMMAND_TYPE
{
    OCT_RESERVED = 1,
    OCT_ORION_FEATURES = 50,
    OCT_ORION_IGNORE_TILES_IN_FILTER = 51,
    OCT_ORION_VERSION = 52,
    OCT_CLOSE_GENERIC_GUMP_WITHOUT_RESPONSE = 100,
    OCT_SELECT_MENU = 101,
    OCT_CAST_SPELL_REQUEST = 102,
    OCT_USE_SKILL_REQUEST = 103,
    OCT_DRAW_STATUSBAR = 104,
    OCT_CLOSE_STATUSBAR = 105,
    OCT_SECURE_TRADE_CHECK = 106,
    OCT_SECURE_TRADE_CLOSE = 107,
    OCT_UNICODE_SPEECH_REQUEST = 108,
    OCT_RENAME_MOUNT_REQUEST = 109,
    OCT_RECONNECT = 110,
    OCT_PLAY_MACRO = 111,
    OCT_MOVE_PAPERDOLL = 112,
    OCT_USE_ABILITY = 113
};

enum ORION_FILE_INDEX
{
    OFI_MAP_0_MUL = 1,
    OFI_MAP_1_MUL,
    OFI_MAP_2_MUL,
    OFI_MAP_3_MUL,
    OFI_MAP_4_MUL,
    OFI_MAP_5_MUL,
    OFI_MAP_0_UOP,
    OFI_MAP_1_UOP,
    OFI_MAP_2_UOP,
    OFI_MAP_3_UOP,
    OFI_MAP_4_UOP,
    OFI_MAP_5_UOP,
    OFI_MAPX_0_UOP,
    OFI_MAPX_1_UOP,
    OFI_MAPX_2_UOP,
    OFI_MAPX_3_UOP,
    OFI_MAPX_4_UOP,
    OFI_MAPX_5_UOP,
    OFI_STAIDX_0_MUL,
    OFI_STAIDX_1_MUL,
    OFI_STAIDX_2_MUL,
    OFI_STAIDX_3_MUL,
    OFI_STAIDX_4_MUL,
    OFI_STAIDX_5_MUL,
    OFI_STATICS_0_MUL,
    OFI_STATICS_1_MUL,
    OFI_STATICS_2_MUL,
    OFI_STATICS_3_MUL,
    OFI_STATICS_4_MUL,
    OFI_STATICS_5_MUL,
    OFI_MAP_DIF_0_MUL,
    OFI_MAP_DIF_1_MUL,
    OFI_MAP_DIF_2_MUL,
    OFI_MAP_DIF_3_MUL,
    OFI_MAP_DIF_4_MUL,
    OFI_MAP_DIF_5_MUL,
    OFI_MAP_DIFL_0_MUL,
    OFI_MAP_DIFL_1_MUL,
    OFI_MAP_DIFL_2_MUL,
    OFI_MAP_DIFL_3_MUL,
    OFI_MAP_DIFL_4_MUL,
    OFI_MAP_DIFL_5_MUL,
    OFI_STA_DIF_0_MUL,
    OFI_STA_DIF_1_MUL,
    OFI_STA_DIF_2_MUL,
    OFI_STA_DIF_3_MUL,
    OFI_STA_DIF_4_MUL,
    OFI_STA_DIF_5_MUL,
    OFI_STA_DIFI_0_MUL,
    OFI_STA_DIFI_1_MUL,
    OFI_STA_DIFI_2_MUL,
    OFI_STA_DIFI_3_MUL,
    OFI_STA_DIFI_4_MUL,
    OFI_STA_DIFI_5_MUL,
    OFI_STA_DIFL_0_MUL,
    OFI_STA_DIFL_1_MUL,
    OFI_STA_DIFL_2_MUL,
    OFI_STA_DIFL_3_MUL,
    OFI_STA_DIFL_4_MUL,
    OFI_STA_DIFL_5_MUL,
    OFI_TILEDATA_MUL,
    OFI_MULTI_IDX,
    OFI_MULTI_MUL,
    OFI_MULTI_UOP,
    OFI_HUES_MUL,
    OFI_VERDATA_MUL,
    OFI_CLILOC_MUL,
    OFI_RADARCOL_MUL,
    OFI_FILES_COUNT
};

enum ORION_GRAPHIC_DATA_TYPE
{
    OGDT_STATIC_ART = 1,
    OGDT_GUMP_ART
};

enum ORION_FEATURE_FLAGS
{
    OFF_DRAW_CHARACTERS_STATUS_IN_WORLD = 0x00000001,
    OFF_CHANGE_TREES_TO_STUMPS = 0x00000002,
    OFF_MARKING_CAVES = 0x00000004,
    OFF_NO_VEGETATION = 0x00000008,
    OFF_NO_FIELDS_ANIMATION = 0x00000010,
    OFF_COLORED_CHARACTERS_STATE = 0x00000020,
    OFF_TILED_FIELDS = 0x00000040,
    OFF_DRAW_AURA = 0x00000080,
    OFF_NO_DRAW_ROOFS = 0x00000100,

    OFF_ALL_FLAGS = 0xFFFFFFFF
};

enum OBJECT_LAYERS
{
    OL_NONE = 0,
    OL_1_HAND,
    OL_2_HAND,
    OL_SHOES,
    OL_PANTS,
    OL_SHIRT,
    OL_HELMET,
    OL_GLOVES,
    OL_RING,
    OL_TALISMAN,
    OL_NECKLACE,
    OL_HAIR,
    OL_WAIST,
    OL_TORSO,
    OL_BRACELET,
    OL_15,
    OL_BEARD,
    OL_TUNIC,
    OL_EARRINGS,
    OL_ARMS,
    OL_CLOAK,
    OL_BACKPACK,
    OL_ROBE,
    OL_SKIRT,
    OL_LEGS,
    OL_MOUNT,
    OL_BUY_RESTOCK,
    OL_BUY,
    OL_SELL,
    OL_BANK
};

enum SPEECH_TYPE
{
    ST_NORMAL = 0,
    ST_BROADCAST,
    ST_EMOTE = 0x02,
    ST_SYSTEM = 0x06,
    ST_SYSTEM_WHITE_NAME = 0x07,
    ST_WHISPER = 0x08,
    ST_YELL = 0x09,
    ST_SPELL = 0x0A,
    ST_GUILD_CHAT = 0x0D,
    ST_ALLIANCE_CHAT = 0x0E,
    ST_COMMAND_PROMT = 0x0F,
    ST_ENCODED_COMMAND = 0xC0,
};

enum TEXT_TYPE
{
    TT_CLIENT = 0,
    TT_SYSTEM,
    TT_OBJECT
};

enum TEXT_ALIGN_TYPE
{
    TS_LEFT = 0,
    TS_CENTER,
    TS_RIGHT
};

enum HTML_TAG_TYPE
{
    HTT_NONE = 0,
    HTT_B,
    HTT_I,
    HTT_A,
    HTT_U,
    HTT_P,
    HTT_BIG,
    HTT_SMALL,
    HTT_BODY,
    HTT_BASEFONT,
    HTT_H1,
    HTT_H2,
    HTT_H3,
    HTT_H4,
    HTT_H5,
    HTT_H6,
    HTT_BR,
    HTT_BQ,
    HTT_LEFT,
    HTT_CENTER,
    HTT_RIGHT,
    HTT_DIV
};

enum PROMPT_TYPE
{
    PT_NONE = 0,
    PT_ASCII,
    PT_UNICODE
};

enum MOUSE_WHEEL_STATE
{
    MWS_UP = 0,
    MWS_DOWN,
    MWS_DOUBLE_CLICK,
    MWS_SCROLL_UP,
    MWS_SCROLL_DOWN
};

enum DRAW_CHARACTERS_STATUS_STATE
{
    DCSS_NO_DRAW = 0,
    DCSS_ABOVE,
    DCSS_UNDER
};

enum DRAW_CHARACTERS_STATUS_CONDITION_STATE
{
    DCSCS_ALWAYS = 0,
    DCSCS_NOT_MAX,
    DCSCS_LOWER
};

enum HIDDEN_CHARACTERS_RENDER_MODE
{
    HCRM_ORIGINAL = 0,
    HCRM_ALPHA_BLENDING,
    HCRM_SPECTRAL_COLOR,
    HCRM_SPECIAL_SPECTRAL_COLOR
};

enum DRAW_AURA_STATE
{
    DAS_NEVER = 0,
    DAS_IN_WARMODE,
    DAS_ALWAYS
};

enum SCREENSHOT_FORMAT
{
    SF_BMP = 0,
    SF_PNG,
    SF_TIFF,
    SF_JPEG
};

enum OBJECT_PROPERTIES_MODE
{
    OPM_AT_ICON = 0,
    OPM_ALWAYS_UP,
    OPM_FOLLOW_MOUSE,
    OPM_SINGLE_CLICK
};

enum CHARACTER_BACKPACK_STYLE
{
    CBS_DEFAULT = 0,
    CBS_SUEDE,
    CBS_POLAR_BEAR,
    CBS_GHOUL_SKIN
};

enum SHADER_DRAW_MODE
{
    SDM_NO_COLOR = 0,
    SDM_COLORED = 1,
    SDM_PARTIAL_HUE = 2,
    SDM_TEXT_COLORED_NO_BLACK = 3,
    SDM_TEXT_COLORED = 4,
    SDM_LAND = 6,
    SDM_LAND_COLORED = 7,
    SDM_SPECTRAL = 10,
    SDM_SPECIAL_SPECTRAL = 11,
    SDM_SHADOW = 12
};

enum STATIC_TILES_FILTER_FLAGS
{
    STFF_CAVE = 0x01,
    STFF_STUMP = 0x02,
    STFF_STUMP_HATCHED = 0x04,
    STFF_VEGETATION = 0x08,
    STFF_WATER = 0x10
};

enum ORION_INTERNAL_PACKET_MESSAGE_TYPE
{
    OIPMT_FILES_TRANSFERED = 1,
    OIPMT_FILE_INFO = 2,
    OIPMT_FILE_INFO_LOCALIZED = 3,
    OIPMT_GRAPHIC_DATA_INFO = 4,
    OIPMT_SKILL_LIST = 100,
    OIPMT_SPELL_LIST = 101,
    OIPMT_MACRO_LIST = 102,
    OIPMT_OPEN_MAP = 103
};

enum DIRECTION_TYPE
{
    DT_N = 0,
    DT_NE,
    DT_E,
    DT_SE,
    DT_S,
    DT_SW,
    DT_W,
    DT_NW
};

enum ANIMATION_GROUPS_TYPE
{
    AGT_MONSTER = 0,
    AGT_SEA_MONSTER,
    AGT_ANIMAL,
    AGT_HUMAN,
    AGT_EQUIPMENT,
    AGT_UNKNOWN
};

enum ANIMATION_GROUPS
{
    AG_NONE = 0,
    AG_LOW,
    AG_HIGHT,
    AG_PEOPLE
};

enum LOW_ANIMATION_GROUP
{
    LAG_WALK = 0,
    LAG_RUN,
    LAG_STAND,
    LAG_EAT,
    LAG_UNKNOWN,
    LAG_ATTACK_1,
    LAG_ATTACK_2,
    LAG_ATTACK_3,
    LAG_DIE_1,
    LAG_FIDGET_1,
    LAG_FIDGET_2,
    LAG_LIE_DOWN,
    LAG_DIE_2,

    LAG_ANIMATION_COUNT
};

enum HIGHT_ANIMATION_GROUP
{
    HAG_WALK = 0,
    HAG_STAND,
    HAG_DIE_1,
    HAG_DIE_2,
    HAG_ATTACK_1,
    HAG_ATTACK_2,
    HAG_ATTACK_3,
    HAG_MISC_1,
    HAG_MISC_2,
    HAG_MISC_3,
    HAG_STUMBLE,
    HAG_SLAP_GROUND,
    HAG_CAST,
    HAG_GET_HIT_1,
    HAG_MISC_4,
    HAG_GET_HIT_2,
    HAG_GET_HIT_3,
    HAG_FIDGET_1,
    HAG_FIDGET_2,
    HAG_FLY,
    HAG_LAND,
    HAG_DIE_IN_FLIGHT,

    HAG_ANIMATION_COUNT
};

enum PEOPLE_ANIMATION_GROUP
{
    PAG_WALK_UNARMED = 0,
    PAG_WALK_ARMED,
    PAG_RUN_UNARMED,
    PAG_RUN_ARMED,
    PAG_STAND,
    PAG_FIDGET_1,
    PAG_FIDGET_2,
    PAG_STAND_ONEHANDED_ATTACK,
    PAG_STAND_TWOHANDED_ATTACK,
    PAG_ATTACK_ONEHANDED,
    PAG_ATTACK_UNARMED_1,
    PAG_ATTACK_UNARMED_2,
    PAG_ATTACK_TWOHANDED_DOWN,
    PAG_ATTACK_TWOHANDED_WIDE,
    PAG_ATTACK_TWOHANDED_JAB,
    PAG_WALK_WARMODE,
    PAG_CAST_DIRECTED,
    PAG_CAST_AREA,
    PAG_ATTACK_BOW,
    PAG_ATTACK_CROSSBOW,
    PAG_GET_HIT,
    PAG_DIE_1,
    PAG_DIE_2,
    PAG_ONMOUNT_RIDE_SLOW,
    PAG_ONMOUNT_RIDE_FAST,
    PAG_ONMOUNT_STAND,
    PAG_ONMOUNT_ATTACK,
    PAG_ONMOUNT_ATTACK_BOW,
    PAG_ONMOUNT_ATTACK_CROSSBOW,
    PAG_ONMOUNT_SLAP_HORSE,
    PAG_TURN,
    PAG_ATTACK_UNARMED_AND_WALK,
    PAG_EMOTE_BOW,
    PAG_EMOTE_SALUTE,
    PAG_FIDGET_3,

    PAG_ANIMATION_COUNT
};

enum NOTORIETY_TYPE
{
    NT_NONE = 0,
    NT_INNOCENT,
    NT_FRIENDLY,
    NT_SOMEONE_GRAY,
    NT_CRIMINAL,
    NT_ENEMY,
    NT_MURDERER,
    NT_INVULNERABLE
};

enum CLICK_OBJECT_TYPE
{
    COT_NONE = 0,
    COT_GAME_OBJECT,
    COT_STATIC,
    COT_GUMP
};

enum MACRO_CODE
{
    MC_NONE = 0,
    MC_SAY,
    MC_EMOTE,
    MC_WHISPER,
    MC_YELL,
    MC_WALK,
    MC_WAR_PEACE,
    MC_PASTE,
    MC_OPEN,
    MC_CLOSE,
    MC_MINIMIZE,
    MC_MAXIMIZE,
    MC_OPEN_DOOR,
    MC_USE_SKILL,
    MC_LAST_SKILL,
    MC_CAST_SPELL,
    MC_LAST_SPELL,
    MC_LAST_OBJECT,
    MC_BOW,
    MC_SALUTE,
    MC_QUIT_GAME,
    MC_ALL_NAMES,
    MC_LAST_TARGET,
    MC_TARGET_SELF,
    MC_ARM_DISARM,
    MC_WAIT_FOR_TARGET,
    MC_TARGET_NEXT,
    MC_ATTACK_LAST,
    MC_DELAY,
    MC_CIRCLE_TRANS,
    MC_CLOSE_GUMPS,
    MC_ALWAYS_RUN,
    MC_KILL_GUMP_OPEN,
    MC_PRIMARY_ABILITY,
    MC_SECONDARY_ABILITY,
    MC_EQUIP_LAST_WEAPON,
    MC_SET_UPDATE_RANGE,
    MC_MODIFY_UPDATE_RANGE,
    MC_INCREASE_UPDATE_RANGE,
    MC_DECREASE_UPDATE_RANGE,
    MC_MAX_UPDATE_RANGE,
    MC_MIN_UPDATE_RANGE,
    MC_DEFAULT_UPDATE_RANGE,
    MC_UPDATE_RANGE_INFO,
    MC_ENABLE_RANGE_COLOR,
    MC_DISABLE_RANGE_COLOR,
    MC_TOGGLE_RANGE_COLOR,
    MC_INVOKE_VIRTURE,
    MC_SELECT_NEXT,
    MC_SELECT_PREVEOUS,
    MC_SELECT_NEAREST,
    MC_ATTACK_SELECTED_TARGET,
    MC_USE_SELECTED_TARGET,
    MC_CURRENT_TARGET,
    MC_TARGET_SYSTEM_ON_OFF,
    MC_TOGGLE_BUICON_WINDOW,
    MC_BANDAGE_SELF,
    MC_BANDAGE_TARGET,
    MC_RESYNCHRONIZE,
};

enum MACRO_SUB_CODE
{
    MSC_NONE = 0,
    MSC_G1_NW,
    MSC_G1_N,
    MSC_G1_NE,
    MSC_G1_E,
    MSC_G1_SE,
    MSC_G1_S,
    MSC_G1_SW,
    MSC_G1_W,
    MSC_G2_CONFIGURATION,
    MSC_G2_PAPERDOLL,
    MSC_G2_STATUS,
    MSC_G2_JOURNAL,
    MSC_G2_SKILLS,
    MSC_G2_MAGE_SPELLBOOK,
    MSC_G2_CHAT,
    MSC_G2_BACKPACK,
    MSC_G2_OWERVIEW,
    MSC_G2_WORLD_MAP,
    MSC_G2_MAIL,
    MSC_G2_PARTY_MANIFEST,
    MSC_G2_PARTY_CHAT,
    MSC_G2_GUILD,
    MSC_G3_ANATOMY,
    MSC_G3_ANIMAL_LORE,
    MSC_G3_ANIMAL_TAMING,
    MSC_G3_ARMS_LORE,
    MSC_G3_BEGGING,
    MSC_G3_CARTOGRAPHY,
    MSC_G3_DETECTING_HIDDEN,
    MSC_G3_ENTICEMENT,
    MSC_G3_EVALUATING_INTELLIGENCE,
    MSC_G3_FORENSIC_EVALUATION,
    MSC_G3_HIDING,
    MSC_G3_INSCRIPTION,
    MSC_G3_ITEM_IDENTIFICATION,
    MSC_G3_MEDITATION,
    MSC_G3_PEACEMAKING,
    MSC_G3_POISONING,
    MSC_G3_PROVOCATION,
    MSC_G3_REMOVE_TRAP,
    MSC_G3_SPIRIT_SPEAK,
    MSC_G3_STEALING,
    MSC_G3_STEALTH,
    MSC_G3_TASTE_IDENTIFICATION,
    MSC_G3_TRACKING,
    MSC_G4_LEFT_HAND,
    MSC_G4_RIGHT_HAND,
    MSC_G5_HONOR,
    MSC_G5_SACRIFICE,
    MSC_G5_VALOR,
    MSC_G6_CLUMSY,
    MSC_G6_CREATE_FOOD,
    MSC_G6_FEEBLEMIND,
    MSC_G6_HEAL,
    MSC_G6_MAGIC_ARROW,
    MSC_G6_NIGHT_SIGHT,
    MSC_G6_REACTIVE_ARMOR,
    MSC_G6_WEAKEN,
    MSC_G6_AGILITY,
    MSC_G6_CUNNING,
    MSC_G6_CURE,
    MSC_G6_HARM,
    MSC_G6_MAGIC_TRAP,
    MSC_G6_MAGIC_UNTRAP,
    MSC_G6_PROTECTION,
    MSC_G6_STRENGTH,
    MSC_G6_BLESS,
    MSC_G6_FIREBALL,
    MSC_G6_MAGIC_LOCK,
    MSC_G6_POISON,
    MSC_G6_TELEKINESIS,
    MSC_G6_TELEPORT,
    MSC_G6_UNLOCK,
    MSC_G6_WALL_OF_STONE,
    MSC_G6_ARCH_CURE,
    MSC_G6_ARCH_PROTECTION,
    MSC_G6_CURSE,
    MSC_G6_FIRE_FIELD,
    MSC_G6_GREATER_HEAL,
    MSC_G6_LIGHTNING,
    MSC_G6_MANA_DRAIN,
    MSC_G6_RECALL,
    MSC_G6_BLADE_SPIRITS,
    MSC_G6_DISPELL_FIELD,
    MSC_G6_INCOGNITO,
    MSC_G6_MAGIC_REFLECTION,
    MSC_G6_MIND_BLAST,
    MSC_G6_PARALYZE,
    MSC_G6_POISON_FIELD,
    MSC_G6_SUMMON_CREATURE,
    MSC_G6_DISPEL,
    MSC_G6_ENERGY_BOLT,
    MSC_G6_EXPLOSION,
    MSC_G6_INVISIBILITY,
    MSC_G6_MARK,
    MSC_G6_MASS_CURSE,
    MSC_G6_PARALYZE_FIELD,
    MSC_G6_REVEAL,
    MSC_G6_CHAIN_LIGHTNING,
    MSC_G6_ENERGY_FIELD,
    MSC_G6_FLAME_STRIKE,
    MSC_G6_GATE_TRAVEL,
    MSC_G6_MANA_VAMPIRE,
    MSC_G6_MASS_DISPEL,
    MSC_G6_METEOR_SWARM,
    MSC_G6_POLYMORPH,
    MSC_G6_EARTHQUAKE,
    MSC_G6_ENERGY_VORTEX,
    MSC_G6_RESURRECTION,
    MSC_G6_AIR_ELEMENTAL,
    MSC_G6_SUMMON_DAEMON,
    MSC_G6_EARTH_ELEMENTAL,
    MSC_G6_FIRE_ELEMENTAL,
    MSC_G6_WATER_ELEMENTAL,

    MSC_G7_HOSTILE,
    MSC_G7_PARTY,
    MSC_G7_FOLLOWER,
    MSC_G7_OBJECT,
    MSC_G7_MOBILE,
    MSC_TOTAL_COUNT
};

enum MACRO_RETURN_CODE
{
    MRC_PARSE_NEXT = 0,
    MRC_BREAK_PARSER,
    MRC_STOP
};

enum GAME_CONSOLE_TEXT_TYPE
{
    GCTT_NORMAL = 0,
    GCTT_YELL,
    GCTT_WHISPER,
    GCTT_EMOTE,
    GCTT_C,
    GCTT_BROADCAST,
    GCTT_PARTY,
    GCTT_GUILD,
    GCTT_ALLIANCE,
    GCTT_PARTY_ACCEPT,
    GCTT_PARTY_DECLINE,
    GCTT_PARTY_ADD,
    GCTT_PARTY_LEAVE
};

enum SEASON_TYPE
{
    ST_SPRING = 0,
    ST_SUMMER,
    ST_FALL,
    ST_WINTER,
    ST_DESOLATION
};

enum ABILITY_TYPE
{
    AT_NONE = 0,
    AT_ARMOR_IGNORE,
    AT_BLEED_ATTACK,
    AT_CONCUSSION_BLOW,
    AT_CRUSHING_BLOW,
    AT_DISARM,
    AT_DISMOUNT,
    AT_DOUBLE_STRIKE,
    AT_INFECTING,
    AT_MORTAL_STRIKE,
    AT_MOVING_SHOT,
    AT_PARALYZING_BLOW,
    AT_SHADOW_STRIKE,
    AT_WHIRLWIND_ATTACK,
    AT_RIDING_SWIPE,
    AT_FRENZIED_WHIRLWIND,
    AT_BLOCK,
    AT_DEFENSE_MASTERY,
    AT_NERVE_STRIKE,
    AT_TALON_STRIKE,
    AT_FEINT,
    AT_DUAL_WIELD,
    AT_DOUBLE_SHOT,
    AT_ARMOR_PIERCE,
    AT_BLADEWEAVE,
    AT_FORCE_ARROW,
    AT_LIGHTNING_ARROW,
    AT_PSYCHIC_ATTACK,
    AT_SERPENT_ARROW,
    AT_FORCE_OF_NATURE,
    AT_INFUSED_THROW,
    AT_MYSTIC_ARC,
    AT_DISROBE
};

#endif
