
Dim Odczyt As Byte
Cls
Lcd "Odczyt = "
Do

I2cinit
I2cstart
I2cwbyte 145
I2crbyte Odczyt , Nack
I2cstop



Loop
End
