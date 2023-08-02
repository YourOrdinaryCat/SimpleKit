#pragma once

// Defines a dependency property for a runtime class.
#define DEPENDENCY_PROPERTY(NAME, TYPE)                              \
[noexcept]                                                           \
static Windows.UI.Xaml.DependencyProperty NAME ## Property { get; }; \
TYPE NAME

// Defines an attached property for a runtime class.
#define ATTACHED_PROPERTY(NAME, TYPE, TARGETTYPE)                    \
[noexcept]                                                           \
static Windows.UI.Xaml.DependencyProperty NAME ## Property { get; }; \
static TYPE Get ## NAME ## (TARGETTYPE target);                      \
static void Set ## NAME ## (TARGETTYPE target, TYPE value)
