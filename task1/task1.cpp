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


int main() {


    return 0;
}
