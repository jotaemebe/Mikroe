
// Voltmeter click module connections
sbit Chip_Select_Direction   at TRISC2_bit;
sbit Chip_Select             at RC2_bit;
// eof Voltmeter click module connections

unsigned int calibration, sum, average;
float voltage, measurement;
char txt[4];
char lower_deo, upper_deo;
unsigned int adc_rd;
float voltage_v;

void system_setup( void );
unsigned int getADC( void );

void main()
{
  system_setup();

  UART2_Write_Text( "Entering While Loop...\r\n" );

  while (1) {

    voltage = 0;

    measurement = getADC() / 2;      // Get ADC result
    voltage = (measurement - calibration) * 33.3405;

    FloatToStr(voltage, txt);

    UART2_Write_Text(txt);
    UART2_Write(32);
    UART2_Write_Text("mV");
    UART2_Write(13);
    UART2_Write(10);

    delay_ms(1000);
  }
}

void system_setup( void )
{

  Chip_Select_Direction = 0;       // Set chip select pin to be output
  Chip_Select = 1;

  UART2_Init(9600);               // Initialize UART module at 9600 bps
  Delay_ms(300);                  // Wait for UART module to stabilize
  UART2_Write_Text( "UART Initialized\r\n" );

  voltage = 0;
  sum = 0;
  measurement = 0;
  calibration = 0;

  Chip_Select = 1;                 // Deselect MCP3204

  //SPI
  SPI3_Init_Advanced( _SPI_MASTER, _SPI_8_BIT, 512, _SPI_SS_DISABLE,
                      _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW,
                      _SPI_ACTIVE_2_IDLE );
  Delay_ms(200);
  UART2_Write_Text( "SPI Initialized\r\n" );

  Delay_ms(300);
  UART2_Write_Text( "SPI Initialized\r\n" );

  calibration = getADC() / 2;

}

unsigned int getADC( void )
{
  // Local variables
  char i, buffer;
  volatile unsigned int read, read1, avrg, sum;

  // Initialize variable
  sum = 0;

  for (i = 0; i < 10; i++ )
  {
    Chip_Select = 0;               // Select MCP3201
    read = SPI3_Read(buffer);      // Get first 8 bits of ADC value
    read1 = SPI3_Read(buffer);     // Get the rest of the ADC value bits
    Chip_Select = 1;               // Deselect MCP3201
    read = ((read & 0x1F) << 8);   // Store the first 8 bits of the ADC value in temporary variable
    read = ((read | read1) >> 1);  // Store the rest of the ADC value bits in temporary variable
    sum = sum + read;              // Sum of the eight ADC readings
  }
  avrg = sum / 10;                 // Average ADC value based on sum of the ADC readings
  return avrg;                     // Returns the average ADC value
}