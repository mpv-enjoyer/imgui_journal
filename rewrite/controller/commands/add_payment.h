#pragma once
#include "icommand.h"

class Add_Payment : public ICommand
{
    Position<Contract> m_contract_pos;
    Price m_price;
    Month m_month;
    std::string m_comment;
public:
    Add_Payment(Position<Contract> contract_pos, Price price, Month month, std::string comment)
    : m_contract_pos(contract_pos), m_price(price), m_month(month), m_comment(comment)
    { }
    Error get_error(const IModel&) const override
    {
        if (m_price.get() < 0) return "Невозможно установить негативную оплату";
        return {};
    }
    void call(IModel& model) override
    {
        model->students()->ref_contract(m_contract_pos).add_payment(m_price, m_month, m_comment);
    }
};