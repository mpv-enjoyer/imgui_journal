#pragma once
#include "price.h"
#include "jtime.h"

class Contract
{
public:
    struct Payment : public Removal_Info
    {
        Price price;
        Month month;
        Mday added_at;
        std::string comment;
    };
private:
    std::vector<Payment> m_payments;
    int m_id;
public:
    Contract(int id) : m_id(id) { }
    int get_number() const { return m_id; }
    const std::vector<Payment>& cref_payments() const { return m_payments; }
    void add_payment(Price price, Month month, std::string comment)
    {
        m_payments.push_back(
            Payment{
                .price = price,
                .month = month,
                .added_at = Mday::make_current(),
                .comment = comment
            }
        );
    }
    void remove_payment(size_t payment_id)
    {
        m_payments[payment_id].remove();
    }
    void restore_payment(size_t payment_id)
    {
        m_payments[payment_id].restore();
    }
};