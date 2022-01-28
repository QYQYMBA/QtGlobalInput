# QtGlobalInput
QtGlobalInput is small library that implements windows global hooks in Qt.

## Structure
All funktions in QtGlobalInput are static and should be called with 
```cpp 
QtGlobalInput::functionName()
```
## Init and deinitialize
### Initialization
Before creating hooks, QtGlobalInput must be initialized with
```cpp 
QtGlobalInput::init(hwnd)
```

Example: 
```cpp
QtGlobalInput::init((HWND)MainWindow::winId());
```
### Deinitialization
If you don't need hooks anymore, you can deinialize QtGlobalInput with 
```cpp 
QtGlobalInput::deinitialize()
```

## Set
All funktions are returning hook id. With this id hook can be removed.
### EventType
```cpp
enum class EventType
{
    All,
    ButtonDown,
    ButtonUp
};
```
### setLlKeyboardHook
```cpp
static uint setLlKeyboardHook(uint vkCode, EventType type, A callback, B obj, bool async = false);
```
1. vkCode - vkCode of the key you want to hook. If vkCode is 0, will hook all key presses
2. type - decides if should listen to key down or key up
3. callback - funktion that will be called when hook triggers. Should have form: 
```cpp
void function(int nCode, WPARAM wParam, LPARAM lParam)
```
4. obj - object that has callback funktion
5. async - if true, callback funktion will be called asynchronous
### waitForKeyPress
```cpp
static uint waitForKeyPress(uint vkCode, EventType type, A callback, B obj, bool async = false);
```
1. vkCode - vkCode of the key you want to hook. If vkCode is 0, will hook all key presses
2. type - decides if should listen to key down or key up
3. callback - funktion that will be called when hook triggers. Should have form:
```cpp
void function(RAWKEYBOARD rawkeyboard)
```
4. obj - object that has callback funktion
5. async - if true, callback funktion will be called asynchronous
### wairForMousePress
```cpp
static uint waitForMousePress(uint vkCode, EventType type, A callback, B obj, bool async = false);
```
1. vkCode - vkCode of the key you want to hook. If vkCode is 0, will hook all key presses
2. type - decides if should listen to key down or key up
3. callback - funktion that will be called when hook triggers. Should have form: 
```cpp
void function(RAWMOUSE rawmouse)
```
4. obj - object that has callback funktion
5. async - if true, callback funktion will be called asynchronous
### setWindowSwitch
```cpp
static uint setWindowSwitch(A callback, B obj)
```
1. callback - funktion that will be called when hook triggers. Should have form - `void function(HWND hwnd)`
2. obj - object that has callback funktion

## Remove
This funktions will remove hook by his id and return true if hook is successfully deleted.
```cpp
1. static bool removeLlKeyboardHook(uint id);
2. static bool removeKeyPress(uint id);
3. static bool removeMousePress(uint id);
4. static bool removeWindowSwitch(uint id);
```
## Example
```cpp
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  QtGlobalInput::init((HWND)MainWindow::winId());
  uint keyPressId = QtGlobalInput::waitForKeyPress(0, EventType::ButtonUp, &MainWindow::handleKey, this, true);
  uint windowSwitchId = QtGlobalInput::setWindowSwitch(&MainWindow::windowSwitched, this);
}

void MainWindow::windowSwitched(HWND hwnd) { ... }

void MainWindow::handleKey(RAWKEYBOARD keyboard) { ... }
```
