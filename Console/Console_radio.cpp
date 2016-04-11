#include "Console_radio.h"
#include "Color.h"
#include "Radio_consts.h"

void Console_radio::init()
{
	radio.begin();
	radio.openReadingPipe(1, addresses[console_id_c]);

	// Set the PA Level low to prevent power supply related issues since this is a
	// getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
	radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
}

bool Console_radio::send_color(const Color& color, int sphere_id)
{
	radio.openWritingPipe(addresses[sphere_id]);

	if (!radio.write(color.c_str(), sizeof(char)*buf_size_c)) {
   	return false;
  }

	Serial.println("Sent Color:");
  Serial.print("\t");
  Serial.print(color.c_str());
  Serial.println("");

	return true;
}

bool Console_radio::receive_color(Color& color, int sphere_id)
{
  radio.startListening();

  unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
  bool timeout = false;                                   // Set up a variable to indicate if a response was received or not
  
  while (!radio.available() ){                             // While nothing is received
    if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
      radio.stopListening();
      Serial.println("timeout");
        radio.stopListening();

      return false;
   }   
  }

	char buf[buf_size_c];
	while (radio.available()) {
		radio.read(buf, sizeof(char)*buf_size_c );
	}

	color = Color(buf);

	radio.stopListening();
  Serial.println("Received Color:");
  Serial.print("\t");
  Serial.print(color.c_str());
  Serial.println("");
  radio.stopListening();

	return true;
}

