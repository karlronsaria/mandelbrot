mandelbrot.exe
==============

This package depends on `sfml`.

- [Help](#help)
- [Saving Screen Captures](#saving-screen-captures)
  - [File Name Parts](#file-name-parts)
  - [Example](#example)
- [Command-line Arguments](#command-line-arguments)
  - [Syntax](#syntax)
  - [Examples](#examples)
- [Images](#images)

Help
----

`T`:  Toggle Display <br />
`P`:  Pause Render <br />
`R`:  Restart Render <br />
`I`:  Change Max Iterations <br />
`J`:  Select Julia Set <br />
`M`:  Back to Mandelbrot Set <br />
<br />
`Esc`:  Back to Default Magnification <br />
<br />
`Ctl`+`S`:  Save Screen Capture <br />
`V`:  Start a Recorded Render <br />
<br />
`Left Click`:  Zoom In <br />
`Right Click`:  Zoom Out <br />
<br />
`Alt`+`Left`:  Go Back in History <br />
`Alt`+`Right`:  Go Forward in History <br />
<br />
`PgUp` &ndash; `PgDn`:  Change Function <br />
`Up` &ndash; `Down`:  Change Algorithm <br />
`Left` &ndash; `Right`:  Change Color Scheme <br />
`[` &ndash; `]`:  Change Threshold <br />
<br />

Saving Screen Captures
----------------------

Screen captures can be saved with the `Ctl`+`S` key combination.
Files will be saved with the \*.png extension in the same directory as the
executable.

### File Name Parts

#### Example

```
2020_06_16_041114_-_1_n3_1_003_36d069039d36d03f_ndb40a70d74dad03f_n74da40a70d74ea3f_n989999999999c93f.png
```

- `2020_06_16_041114`:  date-time stamp
- `1_n3_1_003`:  type, order, magnification, iteration

  - Type:
  
    - type = 0:  Mandelbrot set
    - type = 1:  Julia set
    
  - Order:
  
    - order > 0:  _f_(_z_) = _z_<sup>(order)</sup> + _c_
    - order ≤ 0:  _f_(_z_) = a predefined function
    
  - Magnification
  
    - mag ≥ 0:  image magnified by 5<sup>(mag)</sup>
    - mag < 0:  image demagnified (magnified by 5<sup>(-mag)</sup>)
  
- `36d069039d36d03f_ndb40a70d74dad03f`:  center coordinates in hexadecimal
- `n74da40a70d74ea3f_n989999999999c93f`:  Julia set coordinates in hexadecimal
- `n3`:  -3
<br />

Command-line Arguments
----------------------

Enter the name of a screen capture file to go to its render in the application. <br />

#### Syntax

```xml
mandelbrot.exe <filename[.extension]>
```

#### Examples

```powershell
PS C:\>.\mandelbrot.exe ".\2020_06_16_221907_-_0_n10_0_23_545555555555fd3f_aaaaaaaaaaaaf63f.png"
```

```powershell
PS C:\>.\mandelbrot.exe "0_n10_0_23_545555555555fd3f_aaaaaaaaaaaaf63f"
```
<br />

Images
------

![Main Cardiod](/res/2020_06_14_083030_-_2_0_64_000000000000e0bf_0000000000000000.png)
![Zoom](/res/2020_06_14_083232_-_2_3_64_769646b921b3e8bf_23e155e90106bcbf.png)
