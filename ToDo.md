# Animation
    - name of action
    - frame: load img -> texture : rect
    - move: ???
    - defense : Collision
# Action:
    - key -> action graph

# Struct of client:
    - Struct: init character: 
        + client gửi lên
            char*: name
            bool: player_one? -> true: 1
                            false: 2
        
        + Server gửi về: cả 2 client
            char*: name
            bool: player_one?
            if player1: x_pos, y_pos
            if !player1: x_pos, y_pos
    - Struct: in game:
        + client gửi lên:
            id định danh: biết là tk nào gửi lên
            máu: hàm gọi máu
            triggerAction:
            triggerCollision: