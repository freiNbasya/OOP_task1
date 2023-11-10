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



private:
    std::vector<Flight> flights;

};

int main() {


    return 0;
}
