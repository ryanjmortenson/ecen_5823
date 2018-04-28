#ifndef SRC_SECURITY_DISPLAY_H_
#define SRC_SECURITY_DISPLAY_H_

/*
 * @brief Displays the pin on the LCD for the user to enter on client
 * @param pin to display
 */
void display_pin(uint32_t pin);

/*
 * @brief Displays bond failure
 */
void display_bond_failure();

/*
 * @brief Shutdown the display
 */
void shutdown_display(void);

#endif /* SRC_SECURITY_DISPLAY_H_ */
