

# Class phonos::Voice

**template &lt;VFPairModel vf\_pair, typename ftype&gt;**



[**ClassList**](annotated.md) **>** [**phonos**](namespacephonos.md) **>** [**Voice**](classphonos_1_1_voice.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**DspSetup**](#function-dspsetup) (ftype sampleRate, [**Articulation**](classphonos_1_1_articulation.md) \* art=nullptr) <br> |
|  void | [**Process**](#function-process) (ftype Pin, ftype Q\_chirp=0) <br> |
|  Eigen::Vector&lt; ftype, vf\_pair::get\_half\_N()&gt; | [**ReadEffectiveOpenings**](#function-readeffectiveopenings) () <br> |
|  ftype | [**ReadEpsilonSav**](#function-readepsilonsav) () <br> |
|  Eigen::Vector&lt; ftype, vf\_pair::get\_N()&gt; | [**ReadFoldsDisplacement**](#function-readfoldsdisplacement) () <br> |
|  ftype | [**ReadMeanGlottalFlow**](#function-readmeanglottalflow) () <br> |
|  ftype | [**ReadPressureDrop**](#function-readpressuredrop) () <br> |
|  ftype | [**ReadRadiatedPressure**](#function-readradiatedpressure) () <br> |
|  ftype | [**ReadSupGlottalFlow**](#function-readsupglottalflow) () <br> |
|   | [**Voice**](#function-voice) (ftype samplerate, bool yielding\_walls=false) <br> |
|  ftype | [**get\_lambda\_sav**](#function-get_lambda_sav) () <br> |
|  std::shared\_ptr&lt; [**WebsterFDTD**](classphonos_1_1_webster_f_d_t_d.md)&lt; ftype &gt; &gt; | [**get\_resonator**](#function-get_resonator) () <br> |
|  std::shared\_ptr&lt; vf\_pair &gt; | [**get\_vocal\_folds**](#function-get_vocal_folds) () <br> |
|  void | [**set\_lambda\_sav**](#function-set_lambda_sav) (const ftype & lambda\_sav) <br> |




























## Public Functions Documentation




### function DspSetup 

```C++
void phonos::Voice::DspSetup (
    ftype sampleRate,
    Articulation * art=nullptr
) 
```




<hr>



### function Process 

```C++
void phonos::Voice::Process (
    ftype Pin,
    ftype Q_chirp=0
) 
```




<hr>



### function ReadEffectiveOpenings 

```C++
inline Eigen::Vector< ftype, vf_pair::get_half_N()> phonos::Voice::ReadEffectiveOpenings () 
```




<hr>



### function ReadEpsilonSav 

```C++
inline ftype phonos::Voice::ReadEpsilonSav () 
```




<hr>



### function ReadFoldsDisplacement 

```C++
inline Eigen::Vector< ftype, vf_pair::get_N()> phonos::Voice::ReadFoldsDisplacement () 
```




<hr>



### function ReadMeanGlottalFlow 

```C++
inline ftype phonos::Voice::ReadMeanGlottalFlow () 
```




<hr>



### function ReadPressureDrop 

```C++
inline ftype phonos::Voice::ReadPressureDrop () 
```




<hr>



### function ReadRadiatedPressure 

```C++
inline ftype phonos::Voice::ReadRadiatedPressure () 
```




<hr>



### function ReadSupGlottalFlow 

```C++
inline ftype phonos::Voice::ReadSupGlottalFlow () 
```




<hr>



### function Voice 

```C++
phonos::Voice::Voice (
    ftype samplerate,
    bool yielding_walls=false
) 
```




<hr>



### function get\_lambda\_sav 

```C++
inline ftype phonos::Voice::get_lambda_sav () 
```




<hr>



### function get\_resonator 

```C++
inline std::shared_ptr< WebsterFDTD < ftype > > phonos::Voice::get_resonator () 
```




<hr>



### function get\_vocal\_folds 

```C++
inline std::shared_ptr< vf_pair > phonos::Voice::get_vocal_folds () 
```




<hr>



### function set\_lambda\_sav 

```C++
inline void phonos::Voice::set_lambda_sav (
    const ftype & lambda_sav
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/voice.h`

