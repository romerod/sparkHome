#ifndef ADD_WINECOOLER_H
#define ADD_WINECOOLER_H

class winecooler {
	public:
    void setup();
    void loop();
  private:
		static winecooler* instance;
    int currentTemperature;
    int newTemperature;
    unsigned long lastPress;

    static int pressUp(String command);
    static int pressDown(String command);
    static int setTemperature(String command);
    int setNewTemperature(int newTemp);

    static void upPressed();
    static void downPressed();
    void handlePressed(int tempDelta);
};

#endif
