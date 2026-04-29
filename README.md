Small Unreal Engine 5 C++ gameplay prototype focused on puzzle interaction in imersive way 
Video : 

https://github.com/user-attachments/assets/97700736-951f-4a03-a377-71ce4e6565a0



### APuzzleWall
Responsible for:
- puzzle actor setup and overlap zone
- puzzle-specific logic 
- puzzle camera placement 

### UInteractionComponentPuzzle
Responsible for:
- keeping currently available puzzle reference
- adding/removing puzzle input mapping context
- passing mouse/grab input to active puzzle

### UAnimInstanceSandboxCharacter_CMC
Responsible for:
- blending hand interaction state
- receiving puzzle mode target data for animation


## ActionSystem:
i've added this because the way I handle the puzzle interaction might be a little insane right now, i'll change that 
(I will show more)
Video:

https://github.com/user-attachments/assets/71094711-104f-4a92-9071-9ce0f6859862


https://github.com/user-attachments/assets/8753984b-fad6-448d-93ec-ff10822ca61c






### UAS_ActionSystemComponent 
Responsible for:
- tracking active and available character actions
- starting and stopping actions using Gameplay Tags

### UAS_Action
Responsible for:
- defining action
- providing a way to create new actions in the editor

## Controls

- **Interact**: enter/exit puzzle mode
- **Mouse move**: move hand target while in puzzle mode
- **Left click**: grab interaction point


## Refactor Notes

Planned next steps:
1. Finalize input mapping consistency for mouse move.
2. Stabilize enter/exit state handling.
3. Move UI management out of puzzle actor.
4. add proper rotation for the hand
5. add ability to contrain the movable puzzle object for the easy work on puzzle design
Small Unreal Engine 5 C++ gameplay prototype focused on puzzle interaction in imersive way 

