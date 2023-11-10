#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

class Seat {
public:
    Seat(char row, int seatNumber, double price)
        : row(row), seatNumber(seatNumber), price(price), isBooked(false) {}

    bool isAvailable() const { return !isBooked; }
    void book() { isBooked = true; }
    void unbook() { isBooked = false; }

    char getRow() const { return row; }
    int getSeatNumber() const { return seatNumber; }
    double getPrice() const { return price; }

private:
    char row;
    int seatNumber;
    double price;
    bool isBooked;
};

class Flight {
public:

    Flight(const std::string& date, const std::string& flightNumber, int seatsPerRow, int totalRows, int range1_start, int range1_end,
        int price_range1, int range2_start, int range2_end, int price_range2)
        : date(date), flightNumber(flightNumber), seatsPerRow(seatsPerRow), totalRows(totalRows), range1_start(range1_start),
        range1_end(range1_end), price_range1(price_range1), range2_start(range2_start), range2_end(range2_end), price_range2(price_range2)
    {
        initializeSeats();
    }
    struct Booking {
        std::string flightNumber;
        std::string date;
        std::string seat;
        std::string username;
        double price;
        std::string bookingID;
    };
    void initializeSeats() {
        for (int row = 1; row <= totalRows; row++) {
            for (char seat = 'A'; seat < 'A' + seatsPerRow; seat++) {
                seats.push_back(Seat(seat, row, setSeatPrices(row)));
            }
        }
    }
    double setSeatPrices(int current_row) {
        if (current_row < range2_start) {
            return price_range1;
        }
        else {
            return price_range2;
        }
    }

    bool isSeatAvailable(const std::string& seat) const {
        for (const auto& s : seats) {
            if (s.isAvailable() && s.getRow() == seat[0] && s.getSeatNumber() == std::stoi(seat.substr(1))) {
                return true;
            }
        }
        return false;
    }
    std::string bookSeat(const std::string& seat, const std::string& username) {
        for (auto& s : seats) {
            if (s.isAvailable() && s.getRow() == seat[0] && s.getSeatNumber() == std::stoi(seat.substr(1))) {
                s.book();
                std::string bookingID = flightNumber + "-" + std::to_string(lastBookingID++);
                bookedTickets.push_back({ flightNumber, date, seat, username, s.getPrice(), bookingID });
                return "Confirmed with ID " + bookingID;
            }
        }
        return "Seat not available";

    }
    void unbookSeat(const std::string& bookingID) {
        for (auto& booking : bookedTickets) {
            if (booking.bookingID == bookingID) {
                for (auto& s : seats) {
                    if (s.getRow() == booking.seat[0] && s.getSeatNumber() == std::stoi(booking.seat.substr(1))) {
                        s.unbook();
                        return;
                    }
                }
            }
        }
    }
 

    const std::vector<Seat>& getSeats() const {
        return seats;
    }

    const std::vector<Booking>& getBookedTickets() const {
        return bookedTickets;
    }


    std::string getDate() const { return date; }
    std::string getFlightNumber() const { return flightNumber; }

private:
    std::string date;
    std::string flightNumber;
    int range1_start;
    int range1_end;
    int range2_start;
    int range2_end;

    int seatsPerRow;
    std::vector<Seat> seats;
    int totalRows;
    int price_range1;
    int price_range2;
    std::vector<std::string> bookedSeats;
    int lastBookingID = 10000;


    std::vector<Booking> bookedTickets;
};

class Airline {
public:
    Airline() {}

    void readConfiguration(const std::string& filename) {
        std::ifstream configFile(filename);
        if (!configFile.is_open()) {
            std::cerr << "Failed to open config file." << std::endl;
            return;
        }

        std::string line;
        std::getline(configFile, line);

        while (std::getline(configFile, line)) {
            std::istringstream iss(line);
            std::string date;
            std::string flightNumber;
            int seatsPerRow;
            int totalRows;
            std::string row_range1;
            double price_range1;
            std::string row_range2;
            double price_range2;



            iss >> date >> flightNumber >> seatsPerRow >> totalRows >> row_range1 >> price_range1 >> row_range2 >> price_range2;
            size_t separate_range1 = row_range1.find("-");
            size_t separate_range2 = row_range2.find("-");


            int range1_start = std::stoi(row_range1.substr(0, separate_range1));
            int range1_end = std::stoi(row_range1.substr(separate_range1 + 1));
            int range2_start = std::stoi(row_range2.substr(0, separate_range2));
            int range2_end = std::stoi(row_range2.substr(separate_range2 + 1));


            Flight flight(date, flightNumber, seatsPerRow, totalRows, range1_start, range1_end, price_range1, range2_start, range2_end, price_range2);
            flights.push_back(flight);
        }

        configFile.close();
    }

    void checkSeatAvailability(const std::string& date, const std::string& flightNumber) {
        for (const auto& flight : flights) {
            if (flight.getDate() == date && flight.getFlightNumber() == flightNumber) {
                for (const auto& seat : flight.getSeats()) {
                    if (seat.isAvailable()) {
                        std::cout << seat.getRow() << seat.getSeatNumber() << " " << seat.getPrice() << "$, ";
                    }
                }
                std::cout << std::endl;
                return;
            }
        }
        std::cout << "Flight not found." << std::endl;
    }

    std::string bookTicket(const std::string& date, const std::string& flightNumber, const std::string& seat, const std::string& username) {
        for (auto& flight : flights) {
            if (flight.getDate() == date && flight.getFlightNumber() == flightNumber) {
                if (flight.isSeatAvailable(seat)) {
                    return flight.bookSeat(seat, username);
                }
                else {
                    return "Seat not available";
                }
            }
        }
        return "Flight not found.";
    }

    std::string returnTicket(const std::string& bookingID) {
        for (auto& flight : flights) {
            for (auto& booking : flight.getBookedTickets()) {
                if (booking.bookingID == bookingID) {
                    flight.unbookSeat(bookingID);
                    return "Booking with ID " + bookingID + " has been refunded for " + std::to_string(booking.price) + "$";
                }
            }
        }
        return "Booking with ID " + bookingID + " not found.";
    }

    std::string viewBookingInfo(const std::string& bookingID) {
        for (const auto& flight : flights) {
            for (const auto& booking : flight.getBookedTickets()) {
                if (booking.bookingID == bookingID) {
                    return "Booking ID: " + bookingID + "\n"
                        + "Flight: " + booking.flightNumber + "\n"
                        + "Date: " + booking.date + "\n"
                        + "Seat: " + booking.seat + "\n"
                        + "Username: " + booking.username + "\n"
                        + "Price: " + std::to_string(booking.price) + "$";
                }
            }
        }
        return "Booking with ID " + bookingID + " not found.";
    }

    void viewUserBookings(const std::string& username) {
        for (const auto& flight : flights) {
            for (const auto& booking : flight.getBookedTickets()) {
                if (booking.username == username) {
                    std::cout << "Flight " << booking.flightNumber << ", " << booking.date << ", seat " << booking.seat << ", price " << booking.price << "$, " << username << std::endl;
                }
            }
        }
    }


private:
    std::vector<Flight> flights;

};

int main() {


    return 0;
}
