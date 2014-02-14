Module zigbee

    Public Const RC100_BTN_U = 1
    Public Const RC100_BTN_D = 2
    Public Const RC100_BTN_L = 4
    Public Const RC100_BTN_R = 8
    Public Const RC100_BTN_1 = 16
    Public Const RC100_BTN_2 = 32
    Public Const RC100_BTN_3 = 64
    Public Const RC100_BTN_4 = 128
    Public Const RC100_BTN_5 = 256
    Public Const RC100_BTN_6 = 512

    Public Declare Function zgb_initialize Lib "zigbee.dll" Alias "zgb_initialize" (ByVal devIndex As Integer) As Integer
    Public Declare Sub zgb_terminate Lib "zigbee.dll" Alias "zgb_terminate" ()
    Public Declare Function zgb_tx_data Lib "zigbee.dll" Alias "zgb_tx_data" (ByVal data As Integer) As Integer
    Public Declare Function zgb_rx_check Lib "zigbee.dll" Alias "zgb_rx_check" () As Integer
    Public Declare Function zgb_rx_data Lib "zigbee.dll" Alias "zgb_rx_data" () As Integer
    
End Module
