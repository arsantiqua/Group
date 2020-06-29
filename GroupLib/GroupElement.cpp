#include <stdint.h>
#include <math.h>

#include <bitset>

#include "GroupElement.h"

GroupElement::GroupElement()
{
  // No initialization, so order is currently undefined

  set_order(0);
  value = "";
  inverse = "";
  eveness = -1;  // still uninitialized
}

GroupElement::~GroupElement(void)
{}

GroupElement
GroupElement::operator =(const GroupElement& rhs)
{
  check_element(rhs);
  set_order(rhs.get_order());
  std::string elem_value = rhs.get_element();
  set_element(elem_value);

  return *this;
}

bool
GroupElement::operator ==(const GroupElement& rhs) const
{
  if (rhs.get_order() != get_order())
    {
      GroupError gp(GrpErr_IncompatibleGroupElement);
      throw gp;
    }
  return (get_element() == rhs.get_element());
}

bool
GroupElement::operator !=(const GroupElement& rhs) const
{
  return (get_element() != rhs.get_element());
}


bool
GroupElement::operator <(const GroupElement& rhs) const
{
  return (get_element() < rhs.get_element());
}


bool
GroupElement::operator >(const GroupElement& rhs) const
{
  return (get_element() > rhs.get_element());
}



GroupElement
GroupElement::operator *=(const GroupElement & rhs)
  {
    if (!check_element(rhs))
      {
        GroupError gp(GrpErr_IncompatibleGroupElement);

        throw gp;
      }

    GroupElement new_element;

    std::string rhs_element_value;
    std::string our_element_value;
    std::string new_element_value;

    uint32_t order;

    order = rhs.get_order();

    rhs_element_value = rhs.get_element();
    our_element_value = get_element();

    new_element.set_order(order);

    for (uint32_t i = 0; i < order; i++)
      {
        new_element_value
          += our_element_value[rhs_element_value[i] - '0'];
      }

    set_element(new_element_value);

    return *this;

  }

void
GroupElement::set_order(uint32_t order)
{
  // Reset the value and inverse
  value.clear();
  inverse.clear();

  // set up this element as the identity
  for (uint32_t i = 0; i < order; i++)
    {
      value += i + '0';
      // Inverse of identity is the identity
      inverse += i + '0';
    }
  // It's also even
  eveness = 0;
}

void
GroupElement::set_element(const std::string  str_element_values)
{
  uint32_t order = str_element_values.size();
  GrpErr_t element_err = GrpErr_OK;

  if (order > max_order)
    {
        GroupError gp(GrpErr_ElementOverflow);
      throw gp;
    }

  set_order(order);

  if ((element_err = check_element_values(str_element_values)) == GrpErr_OK)
    {
      value = str_element_values;
      inverse.clear();
    }
  else
    {
      GroupError gp(element_err);
      throw gp;
    }
  create_inverse();
}


bool
GroupElement::check_element(const GroupElement & group_element)
{
  bool rVal = false;

  // is it the same order?
  if (group_element.get_order() == get_order())
    {
      rVal = true;
    }
  return rVal;
}

GrpErr_t
GroupElement::check_element_values(const std::string & element_values)
{
  uint32_t element_order = element_values.size();

  group_order_map.reset();
  for (uint32_t idx = 0; idx < element_order; idx++)
    {
      group_order_map[idx] = true;
    }

  for (uint32_t idx = 0;
       idx < element_order;
       ++idx)
    {
      if (static_cast<uint32_t>(element_values[idx] - '0') >= element_order)
        {
          return GrpErr_ElementValueOutOfRange;
        }
      // if the value at a given location has already been encountered
      if (!group_order_map.test(element_values[idx] - '0'))
        {
         return GrpErr_ElementValueDuplicated;
        }
      group_order_map.flip(element_values[idx] - '0');
    }
  // if there are any values that aren't represented then there is a failure
  if (group_order_map.any())
    {
      return GrpErr_ElementValueMissing;
    }

  return GrpErr_OK;
}

void
GroupElement::clear(void)
{
  value.clear();
  inverse.clear();
  eveness = 0;
}

uint32_t
GroupElement::get_order(void) const
{
  return value.size();
}

// inverse is set in create_inverse
std::string
GroupElement::get_inverse(void)
{
  if (inverse.empty())
    {
      create_inverse();
    }
  return inverse;
}


std::string
GroupElement::get_element(void) const
{
  return value;
}

std::string
GroupElement::create_identity(void)
{
  std::string identity = "";
  uint32_t element_order = get_order();

  for (uint32_t i = 0; i < element_order; i++)
    {
      identity += i + '0';
    }
  return identity;
}

void
GroupElement::create_inverse(void)
{
  std::string inverse_generator;
  uint32_t element_order = get_order();

  if (element_order == 0)
    {
      GroupError gp(GrpErr_UndefinedGroupOrder);
      throw gp;
    }

  inverse_generator = value;
  inverse = create_identity();

  for (size_t i = 0; i < get_order() - 1; ++i)
    {
      for (size_t j = i + 1; j < get_order(); ++j)
        {
          if (inverse_generator[i] > inverse_generator[j])
            {
              eveness++;
              eveness %= 2;

              inverse_generator[j] ^= inverse_generator[i];
              inverse_generator[i] ^= inverse_generator[j];
              inverse_generator[j] ^= inverse_generator[i];
              inverse[j]           ^= inverse[i];
              inverse[i]           ^= inverse[j];
              inverse[j]           ^= inverse[i];

            }
        }
    }
}

bool
GroupElement::is_odd(void) const
{
  if (eveness == -1)
    {
      GroupError error;
      error.set_error_code(GrpErr_UninitializedElement);
      throw(error);
      //create_inverse();
    }
  return eveness % 2;
}
