//---------------------------------------------------------------------------

#pragma once
#include "../list.h"
#include "../sized_list.h"
#include "wrapped_dictionary.h"

//---------------------------------------------------------------------------

class StenoCompiledOrthography;

//---------------------------------------------------------------------------

class StenoReverseSuffixDictionary final : public StenoWrappedDictionary {
public:
  StenoReverseSuffixDictionary(StenoDictionary *dictionary,
                               const uint8_t *baseAddress,
                               const StenoCompiledOrthography &orthography,
                               const StenoDictionary *prefixDictionary,
                               const SizedList<const uint8_t *> suffixes,
                               const List<const uint8_t *> &ignoreSuffixes);

  virtual void ReverseLookup(StenoReverseDictionaryLookup &result) const;
  virtual const char *GetName() const;

private:
  struct Suffix;
  struct ReverseLookupContext;

  const uint8_t *baseAddress;

  const SizedList<Suffix> suffixes;
  const StenoCompiledOrthography &orthography;
  const StenoDictionary *prefixDictionary;

  static SizedList<Suffix>
  CreateSuffixList(const SizedList<const uint8_t *> suffixes,
                   const List<const uint8_t *> &ignoreSuffixes);

  void AddSuffixReverseLookup(ReverseLookupContext &context,
                              StenoReverseDictionaryLookup &result) const;

  bool IsStrokeDefined(const StenoStroke *strokes, size_t prefixStrokeCount,
                       size_t combinedStrokeCount) const;
};

//---------------------------------------------------------------------------
