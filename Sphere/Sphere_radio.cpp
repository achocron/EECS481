#include "Sphere_radio.h"
#include "Radio_consts.h"
#include "Color.h"

static const int this_sphere_id_c = sphereB_id_c;

void Sphere_radio::init()
{
	radio.begin();

	// Set the PA Level low to prevent power supply related issues since this is a
	// getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
	radio.setPALevel(RF24_PA_LOW);

	// Open a writing and reading pipe on each radio, with opposite addresses
	radio.openWritingPipe(addresses[console_id_c]);
	radio.openReadingPipe(1,addresses[this_sphere_id_c]);

	// Start the radio listening for data
	radio.startListening();
}


bool Sphere_radio::send_color(const Color& color)
{
	radio.stopListening();
	
	if (!radio.write(color.c_str(), sizeof(char)*buf_size_c)) {
      radio.startListening();
   		return false;
  	}	
	
	radio.startListening();
	return true;
}

bool Sphere_radio::receive_color(Color& color)
{     
  radio.startListening();

	if (!radio.available()) {
		return false;
	}

	char buf[buf_size_c];
	while (radio.available()) {
		radio.read(buf, sizeof(char) * buf_size_c);
	}

	color = Color(buf);
	return true;
}

bool Sphere_radio::query_mode()
{
  Serial.println("query");
	radio.stopListening();
	
	char query[2] = {this_sphere_id_c + '0', 0};

	if (!radio.write(query, sizeof(char)*2)) {
		radio.startListening();
		return false;
  	}	

	radio.startListening();
  int started_waiting_at = micros();
 	if (!radio.available()) {                   
     return false;   
 	}

 	char buf[2];
	while (radio.available()) {
		radio.read(buf, sizeof(char) * 2);
	}
 Serial.println(buf);

	return (buf[0] == 'G');
}

