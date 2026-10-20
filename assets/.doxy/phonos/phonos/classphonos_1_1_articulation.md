

# Class phonos::Articulation



[**ClassList**](annotated.md) **>** [**phonos**](namespacephonos.md) **>** [**Articulation**](classphonos_1_1_articulation.md)




















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**ReducedSet**](structphonos_1_1_articulation_1_1_reduced_set.md) <br> |






















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Articulation**](#function-articulation) ([**vowels::Vowel**](structphonos_1_1vowels_1_1_vowel.md) const & v=vowels::a) <br> |
|  void | [**FindClosestVowels**](#function-findclosestvowels) (float targetF0, float targetF1, size\_t N, std::vector&lt; [**vowels::Vowel**](structphonos_1_1vowels_1_1_vowel.md) &gt; & closest\_vowels, std::vector&lt; float &gt; & alphas) <br> |
|  void | [**Interpolate2Vowels**](#function-interpolate2vowels) ([**vowels::Vowel**](structphonos_1_1vowels_1_1_vowel.md) const & v1, [**vowels::Vowel**](structphonos_1_1vowels_1_1_vowel.md) const & v2, float alpha) <br> |
|  void | [**InterpolateNVowels**](#function-interpolatenvowels) ([**vowels::Vowel**](structphonos_1_1vowels_1_1_vowel.md) const \* vs, float const \* alphas, size\_t const n) <br> |
|  void | [**SetFromFormants**](#function-setfromformants) (float targetF0, float targetF1, size\_t N=3) <br> |
|  void | [**SetFromVowel**](#function-setfromvowel) ([**vowels::Vowel**](structphonos_1_1vowels_1_1_vowel.md) const & v) <br> |
|  void | [**getAreas**](#function-getareas) (T const \* evaluation\_positions, T \* out, std::size\_t const size) <br> |
|  [**ReducedSet**](structphonos_1_1_articulation_1_1_reduced_set.md) | [**get\_reduced\_set**](#function-get_reduced_set) () <br> |
|  void | [**setFromReducedSet**](#function-setfromreducedset) ([**ReducedSet**](structphonos_1_1_articulation_1_1_reduced_set.md) reduced\_set) <br> |




























## Public Functions Documentation




### function Articulation 

```C++
inline phonos::Articulation::Articulation (
    vowels::Vowel const & v=vowels::a
) 
```




<hr>



### function FindClosestVowels 

```C++
inline void phonos::Articulation::FindClosestVowels (
    float targetF0,
    float targetF1,
    size_t N,
    std::vector< vowels::Vowel > & closest_vowels,
    std::vector< float > & alphas
) 
```




<hr>



### function Interpolate2Vowels 

```C++
inline void phonos::Articulation::Interpolate2Vowels (
    vowels::Vowel const & v1,
    vowels::Vowel const & v2,
    float alpha
) 
```




<hr>



### function InterpolateNVowels 

```C++
inline void phonos::Articulation::InterpolateNVowels (
    vowels::Vowel const * vs,
    float const * alphas,
    size_t const n
) 
```




<hr>



### function SetFromFormants 

```C++
inline void phonos::Articulation::SetFromFormants (
    float targetF0,
    float targetF1,
    size_t N=3
) 
```




<hr>



### function SetFromVowel 

```C++
inline void phonos::Articulation::SetFromVowel (
    vowels::Vowel const & v
) 
```




<hr>



### function getAreas 

```C++
template<typename T>
inline void phonos::Articulation::getAreas (
    T const * evaluation_positions,
    T * out,
    std::size_t const size
) 
```




<hr>



### function get\_reduced\_set 

```C++
inline ReducedSet phonos::Articulation::get_reduced_set () 
```




<hr>



### function setFromReducedSet 

```C++
inline void phonos::Articulation::setFromReducedSet (
    ReducedSet reduced_set
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/articulation.h`

