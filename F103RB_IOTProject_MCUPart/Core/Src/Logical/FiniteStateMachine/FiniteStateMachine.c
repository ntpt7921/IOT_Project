/*
 * fsm.h
 *
 *  Created on: Nov 1, 2022
 *      Author: ntpt
 */

#include <Logical/FiniteStateMachine/FiniteStateMachine.h>

FiniteStateMachine_State FiniteStateMachine_GetNextState(FiniteStateMachine *fsm)
{
    return fsm->get_next_state(fsm->current_state);
}

void Logical_Fsm_SetNextState(FiniteStateMachine *fsm)
{
    FiniteStateMachine_State next_state = FiniteStateMachine_GetNextState(fsm);
    if (next_state == fsm->current_state)
        return;

    if (fsm->set_next_state != NULL)
    {
        fsm->set_next_state(next_state);
    }
    fsm->current_state = next_state;
}

void FiniteStateMachine_DoInState(FiniteStateMachine *fsm)
{
    if (fsm->do_in_state != NULL)
    {
        fsm->do_in_state(fsm->current_state);
    }
}

void FiniteStateMachine_Run(FiniteStateMachine *fsm)
{
	FiniteStateMachine_SetNextState(fsm);
	FiniteStateMachine_DoInState(fsm);
}
