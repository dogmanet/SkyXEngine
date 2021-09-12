#ifndef __XKEYCODES_H
#define __XKEYCODES_H

#define SXI_KEYMAP_SIZE 256

/*! \defgroup sxinput_code_keyboard Коды клавиш клавиатуры
 \ingroup sxinput
@{
*/

#define SIK_OFFS 128

/*! \name F1-F15
@{
*/

#define SIK_F1              0x3B
#define SIK_F2              0x3C
#define SIK_F3              0x3D
#define SIK_F4              0x3E
#define SIK_F5              0x3F
#define SIK_F6              0x40
#define SIK_F7              0x41
#define SIK_F8              0x42
#define SIK_F9              0x43
#define SIK_F10             0x44
#define SIK_F11             0x57
#define SIK_F12             0x58

//!@}

/*! \name Цифры основной клавиатуры
@{
*/

#define SIK_1               0x02
#define SIK_2               0x03
#define SIK_3               0x04
#define SIK_4               0x05
#define SIK_5               0x06
#define SIK_6               0x07
#define SIK_7               0x08
#define SIK_8               0x09
#define SIK_9               0x0A
#define SIK_0               0x0B
//!@}

/*! \name Стрелки
@{
*/

#define SIK_LEFT            (SIK_NUMPAD4 + SIK_OFFS)
#define SIK_RIGHT           (SIK_NUMPAD6 + SIK_OFFS)
#define SIK_DOWN            (SIK_NUMPAD2 + SIK_OFFS)
#define SIK_UP              (SIK_NUMPAD8 + SIK_OFFS)

//!@}

/*! \name Цифровая клавиатура
@{
*/

#define SIK_NUMPAD0         0x52	/*!< 0*/
#define SIK_NUMPAD1         0x4F	/*!< 1*/
#define SIK_NUMPAD2         0x50	/*!< 2*/
#define SIK_NUMPAD3         0x51	/*!< 3*/
#define SIK_NUMPAD4         0x4B	/*!< 4*/
#define SIK_NUMPAD5         0x4C	/*!< 5*/
#define SIK_NUMPAD6         0x4D	/*!< 6*/
#define SIK_NUMPAD7         0x47	/*!< 7*/
#define SIK_NUMPAD8         0x48	/*!< 8*/
#define SIK_NUMPAD9         0x49	/*!< 9*/

#define SIK_NUMLOCK         (0x45 + SIK_OFFS)
#define SIK_DIVIDE          (SIK_SLASH + SIK_OFFS)    /*!< / */
#define SIK_MULTIPLY        0x37    /*!< \* */
#define SIK_SUBTRACT        0x4A    /*!< \- */
#define SIK_ADD             0x4E    /*!< \+ */
#define SIK_DECIMAL         0x53    /*!< \. */
#define SIK_NUMPADENTER     (SIK_ENTER + SIK_OFFS)    /*!< Enter */

//!@}

/*! \name A-Z
@{
*/

#define SIK_A               0x1E
#define SIK_B               0x30
#define SIK_C               0x2E
#define SIK_D               0x20
#define SIK_E               0x12
#define SIK_F               0x21
#define SIK_G               0x22
#define SIK_J               0x24
#define SIK_H               0x23
#define SIK_I               0x17
#define SIK_K               0x25
#define SIK_L               0x26
#define SIK_M               0x32
#define SIK_N               0x31
#define SIK_O               0x18
#define SIK_P               0x19
#define SIK_Q               0x10
#define SIK_R               0x13
#define SIK_S               0x1F
#define SIK_T               0x14
#define SIK_U               0x16
#define SIK_V               0x2F
#define SIK_W               0x11
#define SIK_X               0x2D
#define SIK_Y               0x15
#define SIK_Z               0x2C

//!@}

/*! \name Ctrl
@{
*/
#define SIK_CONTROL         0x88
#define SIK_LCONTROL        0x1D
#define SIK_RCONTROL        (SIK_LCONTROL + SIK_OFFS)
//!@}

/*! \name Shift
@{
*/
#define SIK_SHIFT           0x87
#define SIK_LSHIFT          0x2A
#define SIK_RSHIFT          0x36
//!@}

/*! \name Alt
@{
*/
#define SIK_ALT             0x89
#define SIK_LALT            0x38
#define SIK_RALT            (SIK_LALT + SIK_OFFS)
//!@}

/*! \name Windows key
@{
*/
#define SIK_LWIN            (91 + SIK_OFFS)    /*!< left Windows key */
#define SIK_RWIN            (92 + SIK_OFFS)    /*!< right Windows key */
#define SIK_APPS            (93 + SIK_OFFS)
//!@}

#define SIK_ESCAPE          0x01

#define SIK_LBRACKET        0x1A	/*!< { */
#define SIK_RBRACKET        0x1B	/*!< } */

#define SIK_ENTER           0x1C    /*!< Enter на основной */
#define SIK_SPACE           0x39	/*!< пробел */
#define SIK_CAPSLOCK		0x3A	
#define SIK_TAB             0x0F	

#define SIK_GRAVE           0x29    /*!< \` */
#define SIK_MINUS           0x0C	/*!< \- */
#define SIK_EQUALS          0x0D	/*!< = */
#define SIK_BACKSPACE       0x0E

#define SIK_SEMICOLON       0x27	/*!< ; */
#define SIK_APOSTROPHE      0x28	/*!< ' */

#define SIK_BACKSLASH       0x2B	/*!< \\ */
#define SIK_SLASH           0x35    /*!< / на основной */
#define SIK_COMMA           0x33    /*!< \, на основной */
#define SIK_PERIOD          0x34    /*!< \. на основной */
#define SIK_SCROLLLOCK		0x46    /*!< scroll lock */

#define SIK_HOME            (SIK_NUMPAD7 + SIK_OFFS)    /*!< Home */
#define SIK_END             (SIK_NUMPAD1 + SIK_OFFS)    /*!< End */

#define SIK_PGUP			(SIK_NUMPAD9 + SIK_OFFS)    /*!< Page Up */
#define SIK_PGDOWN			(SIK_NUMPAD3 + SIK_OFFS)    /*!< Page Down */

#define SIK_INSERT          (SIK_NUMPAD0 + SIK_OFFS)    /*!< Insert */
#define SIK_DELETE          (SIK_DECIMAL + SIK_OFFS)    /*!< Delete */

#define SIK_PAUSE           0x45

//!@} sxinput_code_keyboard

//#############################################################################

/*! \defgroup sxinput_code_mouse Коды кнопок мыши
 \ingroup sxinput
@{
*/
#define SIM_START           SIM_LBUTTON
#define SIM_LBUTTON			(SIK_OFFS + 0)	/*!< левая кнопка */
#define SIM_RBUTTON			(SIK_OFFS + 1)	/*!< правая кнопка */
#define SIM_MBUTTON			(SIK_OFFS + 2)	/*!< средняя кнопка (она же скролл) */
#define SIM_XBUTTON1		(SIK_OFFS + 3)	/*!< дополнительная кнопка 1 */
#define SIM_XBUTTON2		(SIK_OFFS + 4)	/*!< дополнительная кнопка 2 */
#define SIM_END             SIM_XBUTTON2
#define SIM_MWHEELUP		(SIK_OFFS + 5)	/*!< колесо вверх */
#define SIM_MWHEELDOWN		(SIK_OFFS + 6)	/*!< колесо вниз */

//!@} sxinput_code_mouse


#endif
