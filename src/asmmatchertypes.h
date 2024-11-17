/**
 * @file
 *
 * @brief Types to store relevant data for asm matching
 *
 * @copyright Unassemblize is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            3 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#include "function.h"
#include <array>
#include <string>
#include <variant>
#include <vector>

namespace unassemblize
{
enum class AsmMatchStrictness
{
    Lenient, // Unknown to known/unknown symbol pairs are treated as match.
    Undecided, // Unknown to known/unknown symbol pairs are treated as undecided, maybe match or mismatch.
    Strict, // Unknown to known/unknown symbol pairs are treated as mismatch.
};

enum class AsmMatchValue
{
    IsMatch,
    IsMaybeMatch,
    IsMaybeMismatch = IsMaybeMatch,
    IsMismatch,
};

struct AsmMismatchInfo
{
    enum MismatchReason : uint16_t
    {
        MismatchReason_Missing = 1 << 0, // Instruction is missing on one side.
        MismatchReason_Invalid = 1 << 1, // Instruction is invalid on one side.
        MismatchReason_JumpLen = 1 << 2, // Jump length is different.
    };

    AsmMatchValue get_match_value(AsmMatchStrictness strictness) const;

    bool is_match() const;
    bool is_mismatch() const;

    bool is_maybe_match() const;
    bool is_maybe_mismatch() const;

    uint16_t mismatch_bits = 0; // Bits representing positions where instructions are mismatching.
    uint16_t maybe_mismatch_bits = 0; // Bits representing positions where instructions are maybe mismatching.
    uint16_t mismatch_reasons = 0;
};
static_assert(sizeof(AsmMismatchInfo) <= 8);

struct AsmLabelPair
{
    std::array<const AsmLabel *, 2> pair; // A pointer can be null.
};

struct AsmInstructionPair
{
    std::array<const AsmInstruction *, 2> pair; // A pointer can be null.
    AsmMismatchInfo mismatch_info;
};

using AsmComparisonRecord = std::variant<AsmLabelPair, AsmInstructionPair>;
using AsmComparisonRecords = std::vector<AsmComparisonRecord>;

AsmMatchStrictness to_asm_match_strictness(const char *str);

struct AsmComparisonResult
{
    uint32_t get_instruction_count() const;
    uint32_t get_match_count(AsmMatchStrictness strictness) const;
    uint32_t get_max_match_count(AsmMatchStrictness strictness) const;
    uint32_t get_mismatch_count(AsmMatchStrictness strictness) const;
    uint32_t get_max_mismatch_count(AsmMatchStrictness strictness) const;

    // Returns 0..1
    float get_similarity(AsmMatchStrictness strictness) const;
    // Returns 0..1
    float get_max_similarity(AsmMatchStrictness strictness) const;

    AsmComparisonRecords records;
    uint32_t label_count = 0;
    uint32_t match_count = 0;
    uint32_t maybe_match_count = 0; // Alias maybe mismatch, could be a match or mismatch.
    uint32_t mismatch_count = 0;
};

/*
 * Pairs a function from 2 executables that can be matched.
 */
struct MatchedFunction
{
    std::string name;
    std::array<Function, 2> function_pair;
    AsmComparisonResult comparison;
};
using MatchedFunctions = std::vector<MatchedFunction>;

/*
 * A single function in an executable that can not be matched with a function of another executable.
 */
struct UnmatchedFunction
{
    std::string name;
    Function function;
};
using UnmatchedFunctions = std::vector<UnmatchedFunction>;

/*
 * Groups function matches of the same compiland or source file together.
 */
struct MatchBundle
{
    std::string name; // Compiland or source file name.
    std::vector<IndexT> matchedFunctions;
    std::array<std::vector<IndexT>, 2> unmatchedFunctions;
};
using MatchBundles = std::vector<MatchBundle>;

enum class MatchBundleType
{
    Compiland, // Functions will be bundled by the compilands they belong to.
    SourceFile, // Functions will be bundled by the source files they belong to (.h .cpp).
    None, // Functions will be bundled into one.
};

MatchBundleType to_match_bundle_type(const char *str);

struct StringPair
{
    std::array<std::string, 2> pair;
};

struct TextFileContent
{
    std::string filename;
    std::vector<std::string> lines;
};

struct TextFileContentPair
{
    std::array<const TextFileContent *, 2> pair;
};

} // namespace unassemblize
