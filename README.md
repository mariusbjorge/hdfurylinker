# HDFury Linker API

- This API is an alternative to the Windows-only API supplied by HDFury. The main difference is cross-platform support, so with this API you can control the HDFury Linker on most Linux based devices.

- Please use this at your own risk - most of the stuff this API is doing is based on guessing. Many of the structures have magic values that I have not yet determined the full meaning of yet.

- It would be great if HDFury themselves could open up a bit about the internals of the interface. I would be particularly interested in learning about the data output from the VIC_MODE command. This command returns the current state of the scaler/converter for the current display mode. I have found input VIC and output VIC as well as pixel clock - but it would be great to know what the other bits mean. The alternative is to do it the brute-force way by capturing the data for lots of different modes and settings.
