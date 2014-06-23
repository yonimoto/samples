#ifndef SSP_SPI_H_
#define SSP_SPI_H_
#ifdef __cplusplus
extern "C" {
#endif



/// Initializes the Pins for SSP Operation in SPI Mode
void initialize_SSPSPI();

/// Exchanges a byte over SPI
/// @param data the byte to send
/// @return the byte captured over SPI
/// @TODO 4.  Optional Optimization: Inline this function
char rxTxByteSSPSPI(char data);

/// Sets the SPI Clock Divider (must be at least 2)
void setSSPSPI_Divider(unsigned int divider);




#ifdef __cplusplus
}
#endif
#endif /* SSP_SPI_H_ */
