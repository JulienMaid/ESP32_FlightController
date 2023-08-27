/*
 * assignationaxes.cpp
 *
 *  Created on: 17 août 2023
 *      Author: julien
 */

#include "assignationaxes.h"
#include "ConstantesPID.h"
#include <string.h>

AssignationAxes::AssignationAxes(const int16_t *i_i16_AxeX, const int16_t *i_i16_AxeY,
    const int16_t *i_i16_AxeZ)
{
	memcpy(m_i16_AxeX, i_i16_AxeX, sizeof(m_i16_AxeX));
	memcpy(m_i16_AxeY, i_i16_AxeY, sizeof(m_i16_AxeY));
	memcpy(m_i16_AxeZ, i_i16_AxeZ, sizeof(m_i16_AxeZ));
}

AssignationAxes::~AssignationAxes()
{
  // TODO Auto-generated destructor stub
}

void AssignationAxes::ReAssignation(const int16_t *i_i16_InputXYZ, int16_t *o_i16_OutputXYZ)
{

  for(auto l_u8_indexBoucle=0; l_u8_indexBoucle<e_RepereOrthonormal_t::NbreAxes; l_u8_indexBoucle++)
  {
	  int16_t * l_pi8_AxeRef = nullptr;

	  switch(l_u8_indexBoucle)
	  {
	  case e_RepereOrthonormal_t::X:
	  {
		  l_pi8_AxeRef = &m_i16_AxeX[0];
		  break;
	  }
	  case e_RepereOrthonormal_t::Y:
	  {
		  l_pi8_AxeRef = &m_i16_AxeY[0];
		  break;
	  }
	  case e_RepereOrthonormal_t::Z:
	  {
		  l_pi8_AxeRef = &m_i16_AxeZ[0];
		  break;
	  }
	  }

	  for(auto l_u8_indexBoucle2=0; l_u8_indexBoucle2<e_RepereOrthonormal_t::NbreAxes; l_u8_indexBoucle2++)
	  {
		  if(l_pi8_AxeRef[l_u8_indexBoucle2] != 0)
		  {
			 o_i16_OutputXYZ[l_u8_indexBoucle2] = l_pi8_AxeRef[l_u8_indexBoucle2] * i_i16_InputXYZ[l_u8_indexBoucle];
		  }
	  }
  }

}
