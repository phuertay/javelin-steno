//---------------------------------------------------------------------------

#include "host_layout.h"
#include "console.h"
#include "key_code.h"
#include "steno_key_code_emitter.h"
#include "str.h"

//---------------------------------------------------------------------------

const HostLayout HostLayout::ansi =
    {
        .name = "us",
        .unicodeMode = UnicodeMode::NONE,
        .asciiKeyCodes =
            {
// clang-format off
// spellchecker: disable
#define LS MODIFIER_L_SHIFT_FLAG
#define BS BACKSPACE
#define APOS APOSTROPHE
#define SCN SEMICOLON
  // NUL       SOH           STX             ETX           EOT    ENQ    ACK    BEL
  0,           0,            0,              0,            0,     0,     0,     0,
  // BS        TAB           LF              VT            FF     CR     SO     SI
  KeyCode::BS, KeyCode::TAB, KeyCode::ENTER, 0,            0,     0,     0,     0,
  // DLE       DC1           DC2             DC3           DC4    NAK    SYN    ETB
  0,           0,            0,              0,            0,     0,     0,     0,
  // CAN       EM            SUB             ESC           FS     GS     RS     US
  0,           0,            0,              KeyCode::ESC, 0,     0,     0,     0,

  //              !               "               #               $               %               &               '
  KeyCode::SPACE, LS|KeyCode::_1, LS|KeyCode::APOS,LS|KeyCode::_3,   LS|KeyCode::_4,   LS|KeyCode::_5,   LS|KeyCode::_7,   KeyCode::APOSTROPHE,
  // (            )               *               +               ,               -               .               /
  LS|KeyCode::_9, LS|KeyCode::_0, LS|KeyCode::_8, LS|KeyCode::EQUAL, KeyCode::COMMA,  KeyCode::MINUS,  KeyCode::DOT,   KeyCode::SLASH,
  // 0            1               2               3               4               5               6               7
  KeyCode::_0,    KeyCode::_1,    KeyCode::_2,    KeyCode::_3,     KeyCode::_4,     KeyCode::_5,     KeyCode::_6,     KeyCode::_7,
  // 8            9               :               ;               <               =               >               ?
  KeyCode::_8,    KeyCode::_9,    LS|KeyCode::SCN,KeyCode::SCN,   LS|KeyCode::COMMA,KeyCode::EQUAL,   LS|KeyCode::DOT, LS|KeyCode::SLASH,
  // @            A               B               C               D               E               F               G
  LS|KeyCode::_2, LS|KeyCode::A,  LS|KeyCode::B,  LS|KeyCode::C,  LS|KeyCode::D,   LS|KeyCode::E,   LS|KeyCode::F,   LS|KeyCode::G,
  // H            I               J               K               L               M               N               O
  LS|KeyCode::H,  LS|KeyCode::I,  LS|KeyCode::J,  LS|KeyCode::K,  LS|KeyCode::L,   LS|KeyCode::M,   LS|KeyCode::N,   LS|KeyCode::O,
  // P            Q               R               S               T               U               V               W
  LS|KeyCode::P,  LS|KeyCode::Q,  LS|KeyCode::R,  LS|KeyCode::S,  LS|KeyCode::T,   LS|KeyCode::U,   LS|KeyCode::V,   LS|KeyCode::W,
  // X            Y               Z               [               \               ]               ^               _
  LS|KeyCode::X,  LS|KeyCode::Y,  LS|KeyCode::Z,  KeyCode::L_BRACKET,  KeyCode::BACKSLASH,  KeyCode::R_BRACKET,  LS|KeyCode::_6,   LS|KeyCode::MINUS,
  // `            a               b               c               d               e               f               g
  KeyCode::GRAVE, KeyCode::A,     KeyCode::B,     KeyCode::C,     KeyCode::D,     KeyCode::E,     KeyCode::F,     KeyCode::G,
  // h            i               j               k               l               m               n               o
  KeyCode::H,     KeyCode::I,     KeyCode::J,     KeyCode::K,     KeyCode::L,     KeyCode::M,     KeyCode::N,     KeyCode::O,
  // p            q               r               s               t               u               v               w
  KeyCode::P,     KeyCode::Q,     KeyCode::R,     KeyCode::S,     KeyCode::T,     KeyCode::U,     KeyCode::V,     KeyCode::W,
  // x            y               z               {               |               }               ~               DEL
  KeyCode::X,     KeyCode::Y,     KeyCode::Z,     LS|KeyCode::L_BRACKET,LS|KeyCode::BACKSLASH,LS|KeyCode::R_BRACKET,LS|KeyCode::GRAVE, 0,
},
#undef LS
#undef BS
#undef APOS
#undef SCN
// spellchecker: enable
        // clang-format on
};

const HostLayout *HostLayouts::activeLayout = &HostLayout::ansi;
const HostLayouts *HostLayouts::instance;

//---------------------------------------------------------------------------

const HostLayoutEntry *
HostLayout::GetSequenceForUnicode(uint32_t unicode) const {
  const HostLayoutEntry *left = begin(entries);
  const HostLayoutEntry *right = end(entries);

  while (left < right) {
    const HostLayoutEntry *mid = left + size_t(right - left) / 2;

    const int compare = (int)unicode - (int)mid->unicode;
    if (compare < 0) {
      right = mid;
    } else if (compare == 0) {
      return mid;
    } else {
      left = mid + 1;
    }
  }
  return nullptr;
}

//---------------------------------------------------------------------------

void HostLayouts::SetData(const HostLayouts &layouts) {
  instance = &layouts;
  activeLayout = layouts.layouts.Front();
}

//---------------------------------------------------------------------------

bool HostLayouts::SetActiveLayout(const char *name) {
  for (const HostLayout *layout : instance->layouts) {
    if (Str::Eq(name, layout->name)) {
      activeLayout = layout;
      return true;
    }
  }
  return false;
}

//---------------------------------------------------------------------------

void HostLayouts::SetHostLayout_Binding(void *context,
                                        const char *commandLine) {
  const char *layoutName = strchr(commandLine, ' ');
  if (!layoutName) {
    Console::Printf("ERR No host layout specified\n\n");
    return;
  }
  ++layoutName;

  if (SetActiveLayout(layoutName)) {
    Console::SendOk();
  } else {
    Console::Printf("ERR Unable to set host layout: \"%s\"\n\n", layoutName);
  }
}

void HostLayouts::ListHostLayouts() {
  Console::Printf("[");
  const char *format = "\"%J\"";
  for (const HostLayout *layout : instance->layouts) {
    Console::Printf(format, layout->GetName());
    format = ",\"%J\"";
  }
  Console::Printf("]\n\n");
}

//---------------------------------------------------------------------------
