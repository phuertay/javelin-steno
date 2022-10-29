//---------------------------------------------------------------------------

#pragma once
#include "chord_history.h"
#include "list.h"
#include "processor/processor.h"
#include "steno_key_code_buffer.h"
#include "steno_key_code_emitter.h"

//---------------------------------------------------------------------------

class Pattern;
class StenoDictionary;
class StenoUserDictionary;

struct StenoConfigBlock;
struct StenoOrthography;

//---------------------------------------------------------------------------

enum StenoEngineMode { NORMAL, ADD_TRANSLATION };

//---------------------------------------------------------------------------

class StenoEngine final : public StenoProcessorElement {
public:
  StenoEngine(const StenoDictionary &dictionary,
              const StenoOrthography &orthography,
              StenoUserDictionary *userDictionary = nullptr);

  void Process(StenoKeyState value, StenoAction action) final;
  void ProcessUndo();
  void ProcessChord(StenoChord chord);
  void Tick() final {}

  void PrintInfo(const StenoConfigBlock *configBlock) const;
  void PrintDictionary() const;

  char *AddSuffix(const char *word, const char *suffix) const;

private:
  static const StenoChord UNDO_CHORD;
  static const size_t SEGMENT_CONVERSION_LIMIT = 32;

  uint32_t strokeCount = 0;
  const StenoDictionary &dictionary;
  const StenoOrthography &orthography;
  const Pattern *patterns;
  StenoUserDictionary *userDictionary;

  StenoState state = {
      .caseMode = StenoCaseMode::NORMAL,
      .joinNext = true,
      .isGlue = false,
      .spaceCharacterLength = 1,
      .spaceCharacter = " ",
  };
  StenoState addTranslationState;

  StenoKeyCodeEmitter emitter;

  ChordHistory history;
  ChordHistory addTranslationHistory;
  StenoKeyCodeBuffer previousKeyCodeBuffer;
  StenoKeyCodeBuffer nextKeyCodeBuffer;

  struct SuffixEntry;

  void AddCandidates(List<SuffixEntry> &candidates, const char *word,
                     const char *suffix) const;
  void ProcessNormalModeUndo();
  void ProcessNormalModeChord(StenoChord chord);
  void InitiateAddTranslationMode();
  void ProcessAddTranslationModeUndo();
  void ProcessAddTranslationModeChord(StenoChord chord);
  bool IsNewline(StenoChord chord) const;
  void EndAddTranslationMode();
  void AddTranslation(size_t newlineIndex);
  void DeleteTranslation(size_t newlineIndex);

  // Returns the number of segments
  size_t UpdateNormalModeTextBuffer(StenoKeyCodeBuffer &buffer,
                                    size_t chordLength);

  size_t UpdateAddTranslationModeTextBuffer(StenoKeyCodeBuffer &buffer);

  static const Pattern *CreatePatterns(const StenoOrthography &orthography);
  static StenoEngineMode mode;

  friend class StenoEngineTester;
};

//---------------------------------------------------------------------------
