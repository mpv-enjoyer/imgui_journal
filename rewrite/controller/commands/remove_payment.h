#pragma once
#include "icommand.h"

class Remove_Payment : public ICommand
{
    Position<Contract> m_contract_pos;
    Position<Payment> m_payment_pos;
public:
    Remove_Payment(Position<Contract> contract_pos, Position<Payment> payment_pos)
    : m_contract_pos(contract_pos), m_payment_pos(payment_pos)
    { }
    Error get_error(const IModel& model) const override
    {
        if (model->students()->cref_contracts()[m_contract_pos].cref_payments()[m_payment_pos].is_removed())
        {
            return "Оплата уже удалена";
        }
        return {};
    }
    void call(IModel& model) override
    {
        model->students()->ref_contract(m_contract_pos).remove_payment(m_payment_pos);
    }
};

class Restore_Payment : public ICommand
{
    Position<Contract> m_contract_pos;
    Position<Payment> m_payment_pos;
public:
    Restore_Payment(Position<Contract> contract_pos, Position<Payment> payment_pos)
    : m_contract_pos(contract_pos), m_payment_pos(payment_pos)
    { }
    Error get_error(const IModel& model) const override
    {
        if (model->students()->cref_contracts()[m_contract_pos].cref_payments()[m_payment_pos].is_removed())
        {
            return "Оплата не удалена";
        }
        return {};
    }
    void call(IModel& model) override
    {
        model->students()->ref_contract(m_contract_pos).restore_payment(m_payment_pos);
    }
};
