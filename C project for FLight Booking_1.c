#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FLIGHTS 5
#define MAX_BOOKINGS 100
#define MAX_PASSENGERS 10
#define MAX_USERS 10

enum SeatClass {
    ECONOMY,
    FIRST_CLASS,
    BUSINESS_CLASS,
    NUM_SEAT_CLASSES
};

struct Flight {
    char flightNumber[20];
    char source[50];
    char destination[50];
    int seatsAvailable[NUM_SEAT_CLASSES];
    float prices[NUM_SEAT_CLASSES];
    char departureTime[10];
    char arrivalTime[10];
};

struct Passenger {
    char name[50];
    char contactNumber[15];
    enum SeatClass seatClass;
};

struct User {
    char username[50];
    char password[50];
};

struct Booking {
    struct User user;  // Added user information to the booking
    struct Flight *flight;
    struct Passenger passengers[MAX_PASSENGERS];
    int numPassengers;
    int numSeats;
    float totalCost; // Added total cost to the booking
    char date[15];
};

struct Booking bookingHistory[MAX_BOOKINGS];
int numBookings = 0;

struct User users[MAX_USERS];
int numUsers = 0;

// Function prototypes
void displayFlight(const struct Flight *flight);
void bookFlight(struct Flight *flight, struct Passenger *passenger, int numSeats, struct Booking *booking);
void viewBookingHistory(const struct Booking *bookings, int numBookings);
void displayMenu();
int authenticateUser();
int isUsernameAvailable(const char *username);

int main() {
    // Sample user data
    users[0] = (struct User){"user1", "password1"};
    users[1] = (struct User){"user2", "password2"};
    numUsers = 2;

    int authenticated = 0;

    do {
        authenticated = authenticateUser();

        if (!authenticated) {
            printf("Authentication failed. Please try again.\n");
        }

    } while (!authenticated);

    struct Flight flights[MAX_FLIGHTS] = {
        {"FL001", "Delhi", "Mumbai", {50, 20, 10}, {1500.50, 3000.50, 5000.50}, "2:00 AM", "6:00 AM"},
        {"FL002", "Bangalore", "Chennai", {30, 15, 5}, {1250.75, 2500.75, 4000.75}, "6:00 AM", "10:00 AM"},
        {"FL003", "Kolkata", "Hyderabad", {40, 10, 5}, {1000.00, 2000.00, 3000.00}, "10:00 AM", "2:00 PM"},
        {"FL004", "Mumbai", "Delhi", {60, 25, 15}, {1800.25, 3500.25, 5500.25}, "2:00 PM", "6:00 PM"},
        {"FL005", "Chennai", "Bangalore", {35, 18, 8}, {1400.75, 2700.75, 4500.75}, "6:00 PM", "10:00 PM"},
    };

    int choice;

    do {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                // Book a Flight
                int selectedFlight;
                printf("Enter the index of the flight you want to book: ");
                scanf("%d", &selectedFlight);

                if (selectedFlight < 0 || selectedFlight >= MAX_FLIGHTS) {
                    printf("Invalid selection.\n");
                    break;
                }

                struct Passenger passenger;
                printf("Enter your name: ");
                scanf("%s", passenger.name);
                printf("Enter your contact number: ");
                scanf("%s", passenger.contactNumber);

                int selectedSeatClass;
                printf("Select Seat Class (0 for Economy, 1 for First Class, 2 for Business Class): ");
                scanf("%d", &selectedSeatClass);
                passenger.seatClass = (enum SeatClass)selectedSeatClass;

                int numSeats;
                printf("Enter the number of seats to book: ");
                scanf("%d", &numSeats);

                struct Booking newBooking;
                printf("Enter the date of the flight (e.g., DD/MM/YYYY): ");
                scanf("%s", newBooking.date);

                // Set the user information in the booking
                strcpy(newBooking.user.username, users[authenticated - 1].username);

                bookFlight(&flights[selectedFlight], &passenger, numSeats, &newBooking);
                bookingHistory[numBookings++] = newBooking;
                break;
            }
            case 2: {
                // View Available Flights
                printf("Available Flights:\n");
                for (int i = 0; i < MAX_FLIGHTS; ++i) {
                    displayFlight(&flights[i]);
                }
                break;
            }
            case 3: {
                // View Booking History
                viewBookingHistory(bookingHistory, numBookings);
                break;
            }
            case 4: {
                // Exit the program
                printf("Exiting the program.\n");
                break;
            }
            default: {
                printf("Invalid choice. Please enter a valid option.\n");
                break;
            }
        }
    } while (choice != 4);

    return 0;
}

// Function to display flight details
void displayFlight(const struct Flight *flight) {
    printf("Flight Number: %s\n", flight->flightNumber);
    printf("Source: %s\n", flight->source);
    printf("Destination: %s\n", flight->destination);
    for (int i = 0; i < NUM_SEAT_CLASSES; ++i) {
        printf("Seats Available (%d): %d\n", i, flight->seatsAvailable[i]);
    }
    printf("Prices:\n");
    for (int i = 0; i < NUM_SEAT_CLASSES; ++i) {
        printf("  Class %d: Rs.%.2f\n", i, flight->prices[i]);
    }
    printf("Departure Time: %s\n", flight->departureTime);
    printf("Arrival Time: %s\n", flight->arrivalTime);
    printf("\n");
}

// Function to book a flight
void bookFlight(struct Flight *flight, struct Passenger *passenger, int numSeats, struct Booking *booking) {
    int *seatsAvailable = &flight->seatsAvailable[passenger->seatClass];

    if (*seatsAvailable >= numSeats) {
        printf("Booking successful!\n");
        *seatsAvailable -= numSeats;

        booking->flight = flight;
        booking->numSeats = numSeats;
        booking->numPassengers = numSeats;

        float totalCost = 0.0;

        // Display booking details
        for (int i = 0; i < numSeats; ++i) {
            printf("Enter name for passenger %d: ", i + 1);
            scanf("%s", booking->passengers[i].name);
            booking->passengers[i].seatClass = passenger->seatClass;
            strcpy(booking->passengers[i].contactNumber, passenger->contactNumber);

            // Calculate and accumulate the total cost
            totalCost += flight->prices[passenger->seatClass];
        }

        // Display total cost
        printf("Total Cost of the Ticket: Rs.%.2f\n", totalCost);

        printf("Departing from: %s\n", flight->source);
        printf("Arriving at: %s\n", flight->destination);
        printf("Number of Seats: %d\n", numSeats);
        printf("Date: %s\n", booking->date);
        printf("Departure Time: %s\n", flight->departureTime);

        // Save total cost in the booking history
        booking->totalCost = totalCost;

    } else {
        printf("Sorry, not enough seats available for the selected flight and seat class.\n");
    }
}

// Function to view booking history
void viewBookingHistory(const struct Booking *bookings, int numBookings) {
    printf("Booking History:\n");

    for (int i = 0; i < numBookings; ++i) {
        printf("Booking %d:\n", i + 1);
        printf("Passenger Details:\n");
        for (int j = 0; j < bookings[i].numPassengers; ++j) {
            printf("  Passenger %d: %s\n", j + 1, bookings[i].passengers[j].name);
        }
        printf("Booked by: %s\n", bookings[i].user.username);
        printf("Total Cost: Rs.%.2f\n", bookings[i].totalCost);
        printf("Departing from: %s\n", bookings[i].flight->source);
        printf("Arriving at: %s\n", bookings[i].flight->destination);
        printf("Number of Seats: %d\n", bookings[i].numSeats);
        printf("Date: %s\n", bookings[i].date);
        printf("Departure Time: %s\n", bookings[i].flight->departureTime);
        printf("\n");
    }
}

// Function to display the menu
void displayMenu() {
    printf("\nMenu:\n");
    printf("1. Book a Flight\n");
    printf("2. View Available Flights\n");
    printf("3. View Booking History\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

// Function to authenticate a user
int authenticateUser() {
    char username[50];
    char password[50];

    printf("Enter your username: ");
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);

    for (int i = 0; i < numUsers; ++i) {
        if (strcmp(username, users[i].username) == 0 && strcmp(password, users[i].password) == 0) {
            return i + 1; // Authentication successful, return the user index + 1
        }
    }

    return 0; // Authentication failed
}

// Function to check if a username is available
int isUsernameAvailable(const char *username) {
    for (int i = 0; i < numUsers; ++i) {
        if (strcmp(username, users[i].username) == 0) {
            return 0; // Username already exists
        }
    }

    return 1; // Username is available
}
