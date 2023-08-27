/*
 * assignationaxes.h
 *
 *  Created on: 17 août 2023
 *      Author: julien
 */

#ifndef ASSIGNATIONAXES_H_
#define ASSIGNATIONAXES_H_

#include <stdint.h>

class AssignationAxes
{
public:
  AssignationAxes(const int16_t *i_i16_AxeX, const int16_t *i_i16_AxeY,
      const int16_t *i_i16_AxeZ);
  virtual ~AssignationAxes();
  AssignationAxes(const AssignationAxes &other) = delete;
  AssignationAxes(AssignationAxes &&other) = delete;
  AssignationAxes& operator=(const AssignationAxes &other) = delete;
  AssignationAxes& operator=(AssignationAxes &&other) = delete;

  void ReAssignation(const int16_t *i_i16_InputXYZ, int16_t *o_i16_OutputXYZ);

private:
  int16_t m_i16_AxeX[3];
  int16_t m_i16_AxeY[3];
  int16_t m_i16_AxeZ[3];
};

#endif /* ASSIGNATIONAXES_H_ */
