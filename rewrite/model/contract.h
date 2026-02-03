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
        AUTOOPS2(Payment, month, added_at);
    };
private:
    Vector_Sortable<Payment> m_payments;
    int m_id;
public:
    Contract(int id) : m_id(id) { }
    int get_number() const { return m_id; }
    const Vector_Sortable<Payment>& cref_payments() const { return m_payments; }
    void add_payment(Price price, Month month, std::string comment)
    {
        m_payments.push_back(
            Ptr<Payment>::make(Payment{
                .price = price,
                .month = month,
                .added_at = Mday::make_current(),
                .comment = comment
            })
        );
    }
    void remove_payment(Pos<Payment> payment_pos)
    {
        m_payments[payment_pos].remove();
    }
    void restore_payment(Pos<Payment> payment_pos)
    {
        m_payments[payment_pos].restore();
    }
};

using Payment = Contract::Payment;