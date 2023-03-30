/*
 * fsm.h
 *
 *  Created on: Nov 1, 2022
 *      Author: ntpt
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include <Logical/FiniteStateMachine/FiniteStateMachine_State.h>
#include "main.h"

/**
 * NOTE:
 * FSM will be represented by a struct, containing the current state and 3 function pointer:
 * - to get the next state
 * - to perform action when changing to the next state
 * - to perform action within a specified state
 *
 * The user will provide 3 corresponding function to invoke the associated action. Multiple
 * FSM can be create and run at the same time.
 *
 * FSM_state_t is declared in fsm_state_list.h
 */

typedef
FiniteStateMachine_State
(*FiniteStateMachine_Function_GetNextState)(FiniteStateMachine_State current_state);

typedef
void
(*FiniteStateMachine_Function_SetNextState)(FiniteStateMachine_State next_state);

typedef
void
(*FiniteStateMachine_Function_DoInState_t)(FiniteStateMachine_State current_state);

typedef struct
{
	FiniteStateMachine_State current_state;
	FiniteStateMachine_Function_GetNextState get_next_state;
	FiniteStateMachine_Function_SetNextState set_next_state;
	FiniteStateMachine_Function_DoInState_t do_in_state;
} FiniteStateMachine;

/**
 * NOTE:
 * Three function prototype below is provided to the user to invoke the action of the FSM
 * The last one is the combination of running the previous 3 function
 */

FiniteStateMachine_State FiniteStateMachine_GetNextState(FiniteStateMachine *fsm);
void FiniteStateMachine_SetNextState(FiniteStateMachine *fsm);
void FiniteStateMachine_DoInState(FiniteStateMachine *fsm);
void FiniteStateMachine_Run(FiniteStateMachine *fsm);

#endif /* INC_FSM_H_ */
