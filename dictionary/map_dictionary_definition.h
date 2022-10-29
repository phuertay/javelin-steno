//---------------------------------------------------------------------------

#pragma once
#include "../chord.h"
#include <stdlib.h>

//---------------------------------------------------------------------------
//
// Data Layout:
//
//  A typical hashmap will use null pointers or a null value for empty
//  entries, but this ends up taking quite a bit of space.
//
//  A smaller hash table could just store all of the data in a contiguous
//  block, and use a single bit in the hash map: 0 = null, 1 = data present.
//
//  However, this would then require scanning the entire set of bits up
//  until the index and counting all 1 bits to determine the offset into
//  the data block.
//
//  The approach used here is to divide the hash table into 32-bit blocks,
//  Each block will have a bit mask, where 0 = null, 1 = present, but
//  there's a `baseOffset` value precalculated, which is a tally of all 1
//  bits in all previous blocks.
//
//  Thus, the offset into the data table can be determined by just doing
//  a population count on the mask up to the bit index being inspected,
//  and adding it to the baseOffset.
//
//  Overall, this reduces the memory requirements from 128 bytes per 32
//  hashmap entries to just 8 bytes -- a 16x savings.
//
//---------------------------------------------------------------------------

struct StenoHashMapEntryBlock {
  uint32_t baseOffset;
  uint32_t mask;
};

struct StenoMapDictionaryStrokesDefinition {
  size_t hashMapSize;

  // Stroke -> text information.
  const uint8_t *data;

  // Hash table information.
  const StenoHashMapEntryBlock *offsets;

  size_t GetOffset(size_t index) const;
  size_t GetEntryCount() const;
  bool PrintDictionary(bool hasData, size_t chordLength, char *buffer,
                       const uint8_t *textBlock) const;
};

//---------------------------------------------------------------------------

constexpr uint32_t STENO_MAP_DICTIONARY_MAGIC = 0x3244534a; // 'JSD2'

struct StenoMapDictionaryDefinition {
  uint32_t magic;
  unsigned int maximumStrokeCount;
  const uint8_t *textBlock;
  const StenoMapDictionaryStrokesDefinition *strokes;
};

//---------------------------------------------------------------------------
