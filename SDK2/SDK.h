#pragma once
#include <Windows.h>

struct ScriptContext;

struct ScriptNPC;
struct ScriptPlayer;
struct ScriptItem;
struct ScriptStat;
struct ScriptGroundItem;

//
// Meta data.
//

#define SDK_VERSION 0

#define MENU_EXECUTE_WALK 0
#define MENU_EXECUTE_PLAYER 1
#define MENU_EXECUTE_NPC1 2
#define MENU_EXECUTE_NPC2 3
#define MENU_EXECUTE_NPC3 4
#define MENU_EXECUTE_NPC4 5
#define MENU_EXECUTE_NPC_ATTACK 6
#define MENU_EXECUTE_NPC_EXAMINE 7
#define MENU_EXECUTE_WIDGET_ITEM 8
#define MENU_EXECUTE_OBJECT2 9
#define MENU_EXECUTE_OBJECT1 10
#define MENU_EXECUTE_WIDGET 11
#define MENU_EXECUTE_GROUND_ITEM 12

#define TRAVERSE_INVALID ((UINT64)-1)
#define TRAVERSE_MOVING (0x888888888000ull)
#define TRAVERSE_DONE (0x888888888001ull)

//
// Widget ids.
//

#define WIDGET_ID_PLAY 0x38a0083
#define WIDGET_ID_LOOT_ALL 0x06560015

//
// Stat indices
//

#define STAT_ATTACK 0
#define STAT_DEFENSE 1
#define STAT_STRENGTH 2
#define STAT_HITPOINTS 3
#define STAT_RANGE 4
#define STAT_PRAYER 5
#define STAT_MAGIC 6
#define STAT_COOKING 7
#define STAT_WOODCUTTING 8
#define STAT_FLETCHING 9
#define STAT_FISHING 10
#define STAT_FIREMAKING 11
#define STAT_CRAFTING 12
#define STAT_SMITHING 13
#define STAT_MINING 14
#define STAT_HERBLORE 15
#define STAT_AGILITY 16
#define STAT_THIEVING 17
#define STAT_SLAYER 18
#define STAT_FARMING 19
#define STAT_RUNECRAFTING 20
#define STAT_HUNTER 21
#define STAT_CONSTRUCTION 22
#define STAT_SUMMONING 23
#define STAT_DUNGEONEERING 24
#define STAT_DIVINATION 25
#define STAT_INVENTION 26
#define STAT_ARCHAEOLOGY 27

//
// Module constants.
//

#define M_NPCS "NPCs"
#define M_PLAYERS "Players"
#define M_OVERLAY "Overlay"
#define M_MENU "Menu"
#define M_MOVEMENT "Movement"
#define M_BANK "Bank"
#define M_INVENTORY "Inventory"
#define M_GROUND_ITEMS "GroundItem"
#define M_INPUT "Input"
#define M_CRT "Crt"

//
// Define the function types for each API module.
//

typedef BOOLEAN(*MenuExecute)(INT8 ExecutionType, UINT32* Args);

//
// Options:
// 1 = first
// 2 = second
// 3 = third
// 4 = fourth
// 5 = attack
// 6 = examine
//
typedef BOOLEAN(*MenuNPCClick)(INT32 ServerIndex, INT32 OptionIndex);
typedef BOOLEAN(*MenuObjectClick)(UINT32 ObjectId, UINT32 X, UINT32 Y, UINT32 Option);
typedef BOOLEAN(*MenuTileClick)(UINT32 X, UINT32 Y);
typedef BOOLEAN(*MenuWidgetClick)(UINT32 Slot, UINT32 Id, UINT32 Option);
typedef BOOLEAN(*MenuWidgetItemClick)(UINT32 Option, UINT32 Index, UINT32 Id);
typedef BOOLEAN(*MenuGroundItemClick)(UINT32 Id, UINT32 X, UINT32 Y, UINT32 Option);

typedef VOID(*MovementWalk)(INT32 X, INT32 Y);

typedef VOID(*InventoryDrop)(INT32 Index);
typedef SIZE_T(*InventoryGetItems)(ScriptItem* Out, SIZE_T Max);
typedef BOOLEAN(*InventoryGetFirstItem)(INT32 Id, ScriptItem* Out);
typedef SIZE_T(*InventoryGetUniqueItems)(ScriptItem* Out, SIZE_T Max);
typedef BOOLEAN(*InventoryIsFull)();
typedef BOOLEAN(*InventoryIsEmpty)();
typedef SIZE_T(*InventoryCount)(INT32 Id);
typedef BOOLEAN(*InventoryStoreCurrency)();

typedef BOOLEAN(*BankIsOpen)();
typedef BOOLEAN(*BankDepositJunk)();
typedef BOOLEAN(*BankDepositAll)();

typedef SIZE_T(*NPCsGetAll)(ScriptNPC* Out, SIZE_T Max);
typedef BOOLEAN(*NPCsGetByServerIndex)(INT32 ServerIndex, ScriptNPC* Out);
typedef BOOLEAN(*NPCsGetClosest)(INT32 Id, ScriptNPC* Out);
typedef BOOLEAN(*NPCsGetClosestAlive)(INT32 Id, ScriptNPC* Out);

typedef ScriptPlayer(*PlayerGetSelf)();

typedef BOOLEAN(*OverlayBegin)(PCSTR Label);
typedef VOID(*OverlayListBox)(PCSTR Label, INT32* Selected, PCSTR* Names, INT32 NameCount);
typedef VOID(*OverlayLabel)(PCSTR Label, PCSTR Fmt, INT32 FmtCount, ...);
typedef VOID(*OverlayCheckbox)(PCSTR Label, BOOLEAN* Checked);
typedef VOID(*OverlayEnd)();

typedef VOID(*InputFocus)();
typedef VOID(*InputUnfocus)();
typedef VOID(*InputLeftClick)(INT32 X, INT32 Y);
typedef VOID(*InputRightClick)(INT32 X, INT32 Y);

typedef ScriptStat(*ClientGetStat)(INT32 StatIndex);

typedef SIZE_T(*GroundItemsGetAll)(ScriptGroundItem* Items, SIZE_T Max);

typedef INT32(*CrtStrcmp)(PCSTR Aa, PCSTR B);
typedef PVOID(*CrtMalloc)(SIZE_T Amount);
typedef PVOID(*CrtCalloc)(SIZE_T ElementCount, SIZE_T ElementSize);

//
// Define the function types for script callbacks.
//

typedef UINT64(*FnScriptOnLoop)();
typedef VOID(*FnScriptOnPaint)();
typedef VOID(*FnScriptOnInventoryItemChanged)(INT32 Slot, INT32 OldId, INT32 NewId);

typedef VOID(*FnOnScriptInit)(ScriptContext* C);
typedef VOID(*FnRegisterScript)(FnOnScriptInit OnInit);

//
// A scriptable vector with 2 integer coordinates.
//
struct ScriptVec2i
{
	INT32 X = 0;
	INT32 Y = 0;
};

//
// A scriptable NPC.
//
struct ScriptNPC
{
	//
	// The NPC's cache index.
	//
	INT32 Id;
	//
	// The NPC's server index.
	//
	INT32 ServerIndex;
	//
	// The NPC's health.
	//
	UINT32 Health;
	//
	// The global position of the npc.
	//
	ScriptVec2i GlobalPosition;
	//
	// Clicks on the NPC.
	//
	MenuNPCClick Click;

	//
	// Determines if this NPC is alive.
	//
	__forceinline bool IsAlive()
	{
		return Health > 0;
	}
};

//
// A scriptable player.
//
struct ScriptPlayer
{
	//
	// The player's server index.
	//
	INT32 ServerIndex = 0;
	//
	// If the player is in an animation.
	//
	BOOLEAN InAnimation = FALSE;
	//
	// The id of the animation the player is in.
	//
	INT32 AnimationId = 0;
	//
	// If the player is in a moving state (walking, running.)
	//
	BOOLEAN IsMoving = FALSE;
	//
	// The global position of the player.
	//
	ScriptVec2i GlobalPosition;

	//
	// Determines if this player is valid.
	//
	__forceinline BOOLEAN IsValid() CONST
	{
		return ServerIndex != -1;
	}
};

//
// A scriptable item.
//
struct ScriptItem
{
	//
	// The internal widget.
	//
	PVOID Internal = NULL;
	//
	// The id of the item.
	//
	INT32 Id = 0;
	//
	// The amount of the item.
	//
	INT32 Amount = 0;
	//
	// The slot the item is in.
	//
	INT32 Slot = 0;

	//
	// Determines if this item is valid.
	//
	__forceinline BOOLEAN IsValid()
	{
		return Id > 0 && Amount > 0;
	}
};

//
// A scriptable ground item.
//
struct ScriptGroundItem
{
	//
	// The item.
	//
	ScriptItem Item;
	//
	// The position of the item.
	//
	ScriptVec2i Position;
};

//
// A scriptable stat.
//
struct ScriptStat
{
	//
	// The current experience.
	//
	UINT32 Xp;
	//
	// The current level.
	//
	UINT32 CurLevel;
	//
	// The maximum level. 
	//
	UINT32 MaxLevel;
};

//
// A piece of named data.
//
struct NamedData
{
	//
	// The name of the data.
	//
	PCSTR Name;
	//
	// The data.
	//
	PVOID Data;
};

struct NamedDataTable
{
	//
	// All entries in the table.
	//
	NamedData* Entries;
	//
	// The number of entries in the table.
	//
	SIZE_T EntryCount;
	//
	// The string compare function.
	//
	CrtStrcmp Strcmp;

	//
	// Inserts a piece of data anywhere into the table.
	//
	__forceinline VOID InsertAnywhere(PCSTR Name, PVOID Data)
	{
		for (auto i = 0ull; i < EntryCount; i++)
		{
			if (!Entries[i].Data)
			{
				Entries[i] = { Name, Data };
				break;
			}
		}
	}

	//
	// Finds a piece of data by name.
	//
	template<typename T>
	__forceinline T Find(PCSTR Name)
	{
		for (auto i = 0; i < EntryCount; i++)
		{
			if (Entries[i].Name && Entries[i].Data && !Strcmp(Entries[i].Name, Name))
			{
				return (T)Entries[i].Data;
			}
		}
		return NULL;
	}
};

//
// The bot SDK.
//
struct SDK
{
	//
	// All modules.
	//
	NamedDataTable Modules;

	//
	// Inserts a module into the SDK.
	//
	__forceinline VOID InsertModule(PCSTR Name, PVOID Data)
	{
		Modules.InsertAnywhere(Name, Data);
	}

	//
	// Finds a function in a module.
	//
	template <typename T>
	__forceinline T FindFunction(PCSTR Module, PCSTR Function)
	{
		auto M = Modules.Find<NamedDataTable*>(Module);
		if (!M)
		{
			return NULL;
		}

		return M->Find<T>(Function);
	}
};

//
// A script context, which will be passed to the entry point of the script.
//
struct ScriptContext
{
	//
	// The SDK for our script. We should save this somewhere.
	//
	SDK ScriptSDK;
	//
	// Called when the script engine loops.
	//
	FnScriptOnLoop OnLoop = NULL;
	//
	// Called when the game is painting the screen.
	//
	FnScriptOnPaint OnPaint = NULL;
	//
	// Called when an item is changed in the inventory.
	//
	FnScriptOnInventoryItemChanged OnInventoryItemChanged = NULL;
};

//
// Bot load context, which we pass to both the bot and the scripts.
// 
// This should simplify mapping in scripts, while keeping everything secure.
//
struct BotContext
{
	//
	// The bot's register script function.
	//
	FnRegisterScript RegisterScript;
};
