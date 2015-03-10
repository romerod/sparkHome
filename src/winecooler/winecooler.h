#ifndef ADD_WINECOOLER_H
#define ADD_WINECOOLER_H

class winecooler {
	public:
    static void setup();
    static void loop();
  private:
		static int currentTemperature;
    static int newTemperature;
		static int lastPublishedTemperature;
    static unsigned long lastPress;

    static int pressUp(String command);
    static int pressDown(String command);
    static int setTemperature(String command);
		static int power(String command);
    static int setNewTemperature(int newTemp);

    static void upPressed();
    static void downPressed();
    static void handlePressed(int tempDelta);
};

#endif
