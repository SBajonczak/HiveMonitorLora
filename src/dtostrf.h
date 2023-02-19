/**
 * From http://forum.arduino.cc/index.php?topic=368720.0
 */
char *dtostrf(double value, int width, unsigned int precision, char *result) {
	sprintf(result,"%d", value);
	return result;
}