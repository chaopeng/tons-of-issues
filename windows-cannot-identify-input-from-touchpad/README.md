# windows cannot identify pointer event from touchpad

This is a demo try to identify pointer event from touchpad. I run this on XPS 
and Surfacebook with Windows 10 and VS2017.

In `wininput/wininout.cpp`, I listen to couple pointer event then call 
`GetPointerInfo`
https://github.com/chaopeng/tons-of-issues/blob/master/windows-cannot-identify-input-from-touchpad/wininput/wininput.cpp#L120
(also tried `GetPointerType`) to get the pointer type. I expect
it returns type=`PT_TOUCHPAD` when I using touchpad, but it returns `PT_MOUSE`.

I also tried to call `GetPointerDevice` but it returns errorcode=6.

I also tried to use `GetCurrentInputMessageSource`
https://github.com/chaopeng/IdentifyInputSource. It also shows the event is from
mouse.

Is there any other API I can try? Or is there missing any configuration?
