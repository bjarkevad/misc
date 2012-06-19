MersenneTwister
===============

A C++ wrapper for the MT pseudo-random number generator.

IMPORTANT NOTE: The C++ wrapper has NOT been tested for its
properties as a random number generator. DO NOT USE THIS FOR
ANYTHING WHERE THE QUALITY OF RANDOM NUMBERS IS IMPORTANT.

** MOST CERTAINLY DO NOT USE THIS FOR CRYPTOGRAPHY! **

This is essentially a thin C++ wrapper around the functionality
provided by mt19937ar.c

The MT algorithm is used extensively in physics. C++ is also used
extensively in physics. There are other ways to obtain the MT
algorithm, e.g. Boost, ROOT... So don't use this one.
