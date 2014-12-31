%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%                 Sockets                   %%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

:-use_module(library(sockets)).
:-use_module(library(random)).
:-use_module(library(lists)).

port(60070).

% launch me in sockets mode
server:-
        port(Port),
        socket_server_open(Port, Socket),
        socket_server_accept(Socket, _Client, Stream, [type(text)]),
        write('Accepted connection'), nl,
        start(Stream),
        socket_server_close(Socket).

% wait for commands
serverLoop(Stream) :-
        repeat,
        read(Stream, ClientMsg),
        write('Received: '), write(ClientMsg), nl,
        parse_input(ClientMsg, MyReply,Stream),
        format(Stream, '~q.~n', [MyReply]),
        write('Wrote: '), write(MyReply), nl,
        flush_output(Stream),
        (ClientMsg == quit; ClientMsg == end_of_file), !.

parse_input(start,start-game,S) :-
        start(S).
        
parse_input(quit, ok-bye,_) :- !.
                
comando(Arg1, Arg2, Answer) :-
        write(Arg1), nl, write(Arg2), nl,
        Answer = '(5)'.


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%                   Stack                   %%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%




start(Stream):- 
        createBoardInput(Lines,Columns,Stream),
        createBoard(Brd,Lines,Columns),
        selectGameModeInput(Mode,AIInt),
        printBoard(Brd),
        write('Press enter to start'),
        ((Mode == 'AI-AI';Mode == 'AI-H')->
         get_char(_);!),
        Column is Columns-1,
        Line is Lines-1, 
        game('X',0,Column,Line,0,Brd,0,0,'N','N',Mode,[Lines,Columns|[]],AIInt).

%selectGameModeInput(-PlayerVsPlayer,-LevelOfIntelligence)
selectGameModeInput(Mode,AIInt):-
        write('In which mode do you want to play?(ex: H-H or AI-H)\n'),
        get_char(_),
        read_line([X1,X2|Xs]),
        name(Mode,[X1,X2|Xs]),
         write('How should AI be?(Random or Smart)\n'),
         read_line(Int),
         name(AIInt,Int).

%createBoardInput(-Number of Lines,-Number of Columns)
createBoardInput(Lines,Columns,Stream):-
        write('How many lines will the board have?\n'),
        format(Stream, '~q.~n', ['Size-Lines']),
        read(Stream,Lines),
        flush_output(Stream),
        write('How many columns will the board have?\n'),
        format(Stream, '~q.~n', ['Size-Columns']),
        read(Stream,Columns),
        flush_output(Stream),
        integer(Lines),
        integer(Columns),
        Lines > 1,
        Columns > 1.


%createBoard(-Board,+Number of Lines,+Number of Columns)
createBoard(Brd,Lines,Columns):-
        length(Brd1,Lines),
        createMatrix(Brd1,Columns),
        replaceM(0,Columns-1,['X'|1],Brd1,BrdT),
        replaceM(Lines-1,0,['Y'|1],BrdT,Brd).
                    
%createMatrix(-Resulting Board,+Number of Columns)
createMatrix([],_Columns).

createMatrix([H|T],Columns):-
        createLine(H,Columns),
        createMatrix(T,Columns).

%createLine(-Single List represting each line of the Board,+Number of elements in the list)
createLine(L,Nmb) :- 
        length(L,Nmb),
        fill(L).

%fill(+List to be filled with 1s)     
fill([]).
fill([1|T]) :-fill(T).

%printBoard(+Board)
printBoard([X|Xs]):- nl,nl,printHead(X),printMatrix([X|Xs]),nl.

printMatrix([]).
printMatrix([X|Xs]) :-
        nl,
        printHead1(X),
        printLine(X),
        nl,
        printTail(X),
        printMatrix(Xs).

printLine([]):- write('|').
printLine([X|Xs]) :-
        integer(X),
        write('|  '),
        write(X),
        write('  '),
        printLine(Xs).

printLine([X|Xs]) :-
        X == '$',
        write('|  '),
        write(X),
        write('  '),
        printLine(Xs).

printLine([X|Xs]) :-
        X == '#',
        write('|  '),
        write(X),
        write('  '),
        printLine(Xs).


printLine([X|Xs]) :-
        \+ integer(X),
        write('|'),
        write(X),
        printLine(Xs).

%prints top of each cell
printHead([]).

printHead([_X|Xs]) :-
        write(' _____'),
        printHead(Xs).

printHead1([]) :- write('|'),nl.

printHead1([_X|Xs]) :-
        write('|     '),
        printHead1(Xs).

%prints bottom of each cell
printTail([]):- write('|').

printTail([_X|Xs]) :-
        write('|_____'),
        printTail(Xs).

%getElementM(+Line of element,+Column of element,+Board,-Element)
getElementM(Line,Column,[_H|T],Z) :-
    Line > 0,
    Line1 is Line - 1,
    getElementM(Line1, Column, T, Z).

getElementM(0,Column,[H|_T],Z) :-
    getElementL(Column, H, Z).

getElementL(Column, [_H | T],Z) :-
    Column > 0,
    Column1 is Column - 1,
    getElementL(Column1, T, Z).

getElementL(0, [H | _T], H):-!.
                       
%replaceM(+Line of element to be replaced,+Column of element to be replaced,+Element to put in Board,+Board,-ResultingBoard])
replaceM(Line,Column,X,[List|Rest],[List|NewList]):- Line > 0,
    Line1 is Line - 1,
    replaceM(Line1, Column,X, Rest, NewList).

replaceM(0, Column,X,[List|Rest],[List1|Rest]) :-
    replaceL(Column,X, List, List1).

replaceL(Column, X,[H|T], [H|T1]) :-
    Column > 0,
    Column1 is Column - 1,
    replaceL(Column1,X, T, T1).

replaceL(0,X, [_H|T], [X|T]):-!.
  
%gameOver(+Player that won)
gameOver(Pl) :-
        nl,
        write('Player '), write(Pl), write(' has won!!!').

%game(+Player,+LineOfX,+ColumnOfX,+LineOfY,+ColumnOfY,+Board,+PointsOfX,+PointsOfY,+PrevChoiceOfX,+PrevChoiceOfY,+Mode,+BoardSize,+AILevel)
game('X',LineX,ColumnX,LineY,ColumnY,Brd,PointsX,PointsY,PrevChoiceX,PrevChoiceY,Mode,BrdSize,AILevel):-
        PointsY < 4,
        printPoints(PointsX,PointsY),
        (Mode == 'H-H'->
         gameLogic('X','$',LineX,ColumnX,NewLine,NewColumn,LineY,ColumnY,Brd,BrdR,PointsX,NewPoints,PrevChoiceX,'H',BrdSize,AILevel),!,
         game('Y',NewLine,NewColumn,LineY,ColumnY,BrdR,NewPoints,PointsY,PrevChoiceX,PrevChoiceY,Mode,BrdSize,AILevel);!),
        (Mode == 'H-AI'->
         gameLogic('X','$',LineX,ColumnX,NewLine,NewColumn,LineY,ColumnY,Brd,BrdR,PointsX,NewPoints,PrevChoiceX,'H',BrdSize,AILevel),!,
         game('Y',NewLine,NewColumn,LineY,ColumnY,BrdR,NewPoints,PointsY,PrevChoiceX,PrevChoiceY,'AI-H',BrdSize,AILevel);!),
        (Mode == 'AI-H'->
         gameLogic('X','$',LineX,ColumnX,NewLine,NewColumn,LineY,ColumnY,Brd,BrdR,PointsX,NewPoints,PrevChoiceX,'AI',BrdSize,AILevel),!,
         game('Y',NewLine,NewColumn,LineY,ColumnY,BrdR,NewPoints,PointsY,PrevChoiceX,PrevChoiceY,'H-AI',BrdSize,AILevel);!),
        (Mode == 'AI-AI'->
         gameLogic('X','$',LineX,ColumnX,NewLine,NewColumn,LineY,ColumnY,Brd,BrdR,PointsX,NewPoints,PrevChoiceX,'AI',BrdSize,AILevel),!,
         game('Y',NewLine,NewColumn,LineY,ColumnY,BrdR,NewPoints,PointsY,PrevChoiceX,PrevChoiceY,Mode,BrdSize,AILevel);!).

game('Y',LineX,ColumnX,LineY,ColumnY,Brd,PointsX,PointsY,PrevChoiceX,PrevChoiceY,Mode,BrdSize,AILevel):-
        PointsX < 4,
        printPoints(PointsX,PointsY),
        (Mode == 'H-H'->
         gameLogic('Y','#',LineY,ColumnY,NewLine,NewColumn,LineX,ColumnX,Brd,BrdR,PointsY,NewPoints,PrevChoiceY,'H',BrdSize,AILevel),!,
         game('X',LineX,ColumnX,NewLine,NewColumn,BrdR,PointsX,NewPoints,PrevChoiceX,PrevChoiceY,Mode,BrdSize,AILevel);!),
        (Mode == 'H-AI'->
         gameLogic('Y','#',LineY,ColumnY,NewLine,NewColumn,LineX,ColumnX,Brd,BrdR,PointsY,NewPoints,PrevChoiceY,'H',BrdSize,AILevel),!,
         game('X',LineX,ColumnX,NewLine,NewColumn,BrdR,PointsX,NewPoints,PrevChoiceX,PrevChoiceY,'AI-H',BrdSize,AILevel);!),
        (Mode == 'AI-H'->
         gameLogic('Y','#',LineY,ColumnY,NewLine,NewColumn,LineX,ColumnX,Brd,BrdR,PointsY,NewPoints,PrevChoiceY,'AI',BrdSize,AILevel),!,
         game('X',LineX,ColumnX,NewLine,NewColumn,BrdR,PointsX,NewPoints,PrevChoiceX,PrevChoiceY,'H-AI',BrdSize,AILevel);!),
        (Mode == 'AI-AI'->
         gameLogic('Y','#',LineY,ColumnY,NewLine,NewColumn,LineX,ColumnX,Brd,BrdR,PointsY,NewPoints,PrevChoiceY,'AI',BrdSize,AILevel),!,
         game('X',LineX,ColumnX,NewLine,NewColumn,BrdR,PointsX,NewPoints,PrevChoiceX,PrevChoiceY,Mode,BrdSize,AILevel);!).

        
game('X',_LineX,_ColumnX,_LineY,_ColumnY,_Brd,_PointsX,4,_PrevChoiceX,_PrevChoiceY,_Mode,_BrdSize,_AILevel):-
        gameOver('Y').

game('Y',_LineX,_ColumnX,_LineY,_ColumnY,_Brd,4,_PointsY,_PrevChoiceX,_PrevChoiceY,_Mode,_BrdSize,_AILevel):-
        gameOver('X').

%gameLogic(+Player,+SymbolOfComplete,+LineOfPlayer,+ColumnOfPlayer,-NewLineOfPlayer,-NewColumnOfPlayer,+LineOfEnemy,+ColumnEnemy,+Board,-ResultingBoard,+PointsOfPlayer,-NewPointsOfPlayer,+PrevChoiceOfPlayer,+TypeOfPlayer,+BoardSize,+AILevel)
gameLogic(Pl,Symbol,Line,Column,NewLine,NewColumn,LineE,ColumnE,Brd,BrdR,Points,NewPoints,PrevChoice,'H',BrdSize,AILevel):-
        readInput(Pl,Dir,Nmb,Dsk),!,
        (validateMove(Line,Column,NewLine,NewColumn, LineE,ColumnE,Dir,Nmb,Dsk,Brd,NewPrevStack,NewNextStack,Symbol,Points,NewPoints,PrevChoice)->
        move(Pl,Line,Column,NewLine,NewColumn,Brd,BrdR,NewPrevStack,NewNextStack),
        printBoard(BrdR);
        gameLogic(Pl,Symbol,Line,Column,NewLine,NewColumn,LineE,ColumnE,Brd,BrdR,Points,NewPoints,PrevChoice,'H',BrdSize,AILevel)).

gameLogic(Pl,Symbol,Line,Column,NewLine,NewColumn,LineE,ColumnE,Brd,BrdR,Points,NewPoints,PrevChoice,'AI',[MaxLine,MaxColumn|[]],'Random'):-
        generateRandomInput(Dir,Nmb,Dsk,MaxColumn,MaxLine),!,
        (validateMove(Line,Column,NewLine,NewColumn, LineE,ColumnE,Dir,Nmb,Dsk,Brd,NewPrevStack,NewNextStack,Symbol,Points,NewPoints,PrevChoice)->
        move(Pl,Line,Column,NewLine,NewColumn,Brd,BrdR,NewPrevStack,NewNextStack),
        printBoard(BrdR),
        advance(Dir,Nmb,Dsk); 
        gameLogic(Pl,Symbol,Line,Column,NewLine,NewColumn,LineE,ColumnE,Brd,BrdR,Points,NewPoints,PrevChoice,'AI',[MaxLine,MaxColumn|[]],'Random')).

gameLogic(Pl,Symbol,Line,Column,NewLine,NewColumn,LineE,ColumnE,Brd,BrdR,Points,NewPoints,PrevChoice,'AI',[MaxLine,MaxColumn|[]],'Smart'):-
        getAvailableMoves(Moves,Brd,Line,Column,MaxLine,MaxColumn),
        getElementM(Line,Column,Brd,[_H|PrevStack]),
        chooseValidPlay(Moves,Nmb,Dsk,Dir,Brd,PrevStack,Line,Column,NewLine,NewColumn,LineE,ColumnE,NewPrevStack,NewNextStack,Symbol,Points,NewPoints,PrevChoice),
        move(Pl,Line,Column,NewLine,NewColumn,Brd,BrdR,NewPrevStack,NewNextStack),
        printBoard(BrdR),
        write('Player '),write(Pl),write('s turn (AI)\n'),
        advance(Dir,Nmb,Dsk).



%chooseValidPlay(+AvailableMoves,-NumberOfTravel,-CarryingDisk,-Direction,+Board,+StackWherePlayerIs,+LineOfPlayer,+ColumnOfPlayer,-NewLineOfPlayer,-NewColumnOfPlayer,+LineOfEnemy,+ColumnEnemy,-StackAfterPlayerLeaves,-StackAfterPlayerArrives,+SymbolOfComplete,+PointsOfPlayer,+NewPointsOfPlayer,+PrevChoiceOfPlayer)
chooseValidPlay(Moves,Nmb,Dsk,Dir,Brd,PrevStack,Line,Column,NewLine,NewColumn,LineF,ColumnF,NewPrevStack,NewNextStack,Symbol,Points,NewPoints,PrevChoice):-
        decidePlay(Moves,Play,PrevStack),
        processPlay(Play,Nmb,Dsk,Dir,Line,Column),
        (validateMove(Line,Column,NewLine,NewColumn, LineF,ColumnF,Dir,Nmb,Dsk,Brd,NewPrevStack,NewNextStack,Symbol,Points,NewPoints,PrevChoice)->
         !;
         deleteInvalid(Moves,Play,MovesR),
          chooseValidPlay(MovesR,_Nmb,_Dsk,_Dir,Brd,PrevStack,Line,Column,NewLine,NewColumn,LineF,ColumnF,NewPrevStack,NewNextStack,Symbol,Points,NewPoints,PrevChoice)).

%deleteInvalid(+AvailableMoves,+PlayToDelete,-ResultingListOfMoves)
deleteInvalid(Moves,[H|_T],MovesR):-
        delete(Moves,H,MovesR).

%generateRandomInput(-DirectionOfTravel,-NumberOfTravel,-CarryingDisk,+NumberOfColumnsInBoard,+NumberOfLinesInBoard)
generateRandomInput(Dir,Nmb,Dsk,MaxColumn,MaxLine):-
        random_select(Dir, ['U','D','L','R'], _Rest1),
        random_select(Dsk, ['Y','N'],_Rest),
        ((Dir == 'U';Dir == 'D')->
        random(1,MaxLine,Nmb);
        random(1,MaxColumn,Nmb)).

%processPlay(+Play,-NumberOfTravel,-CarryingDisk,-DirectionOfTravel,+LineOfPlayer,+ColumnOfPlayer)
processPlay([Play,Pri],Nmb,'Y',Dir,Line,Column):-
        Pri > 2,!,
        calculateMove(Play,Nmb,Dir,Line,Column). 
        
processPlay([Play,Pri],Nmb,'N',Dir,Line,Column):-
        Pri =< 2,!,
        calculateMove(Play,Nmb,Dir,Line,Column).
        
%calculateMove(+Play,-NumberOfTravel,-DirectionOfTravel,+LineOfPlayer,+ColumnOfPlayer)
calculateMove([_Value,LineF,Column|_T],Nmb,'R',LineF,ColumnF):-
              Column - ColumnF > 0,
              Nmb is Column - ColumnF.

calculateMove([_Value,Line,ColumnF|_T],Nmb,'D',LineF,ColumnF):-
              Line - LineF > 0,
              Nmb is Line - LineF.

calculateMove([_Value,Line,ColumnF|_T],Nmb,'U',LineF,ColumnF):-
              LineF - Line > 0,
              Nmb is LineF - Line.

calculateMove([_Value,LineF,Column|_T],Nmb,'L',LineF,ColumnF):-
              ColumnF - Column > 0,
              Nmb is ColumnF - Column.                                                
        
%getAvailableMoves(-ListOfAvailableMoves,+Board,+LineOfPlayer,+ColumnOfPlayer,+NumberOfColumnsInBoard,+NumberOfLinesInBoard)
getAvailableMoves(Moves,Brd,LineF,ColumnF,MaxLine,MaxColumn):-
        getAvailableMovesL(_Moves0,Moves1,Brd,MaxLine,0,ColumnF),
        getAvailableMovesC(Moves1,Moves,Brd,MaxColumn,LineF,0).

getAvailableMovesL(Moves,Moves,_Brd,MaxLine,MaxLine,_Column):-!.

getAvailableMovesL(Moves,MovesR,Brd,MaxLine,Line,Column):-
        Line < MaxLine,
        getElementM(Line,Column,Brd,X),
        add([X,Line,Column],Moves,MovesT),
        Line1 is Line + 1,
        getAvailableMovesL(MovesT,MovesR,Brd,MaxLine,Line1,Column).

getAvailableMovesC(Moves,Moves,_Brd,MaxColumn,_Line,MaxColumn):-!.

getAvailableMovesC(Moves,MovesR,Brd,MaxColumn,Line,Column):-
        Column < MaxColumn,
        getElementM(Line,Column,Brd,X),
        add([X,Line,Column],Moves,MovesT),
        Column1 is Column + 1,
        getAvailableMovesC(MovesT,MovesR,Brd,MaxColumn,Line,Column1).
        
%add(+ElementToAdd,+List,-ListContainingElement)
add(X,[],[X]):-!.
add(X,[A|L],[A|L1]):-
 add(X,L,L1).

%decidePlay(+ListOfAvailableMoves,-Play,+HouseWherePlayerIs)
decidePlay(Moves,[Play,R],Piece):-
        assignPriority(Moves,Priorities,Piece),
        max(Priorities,0,0,0,R,C),!,
        getElementL(C,Moves,Play).

%max(+List,+Value,+Count,+IndexOfCurrentMax,-MaxValue,-IndexOfMax)
max([],R,_Count,C,R,C):-!.        

max([H|T],Value,Count,_CountV,R,C) :-
        H > Value,!,
        Count1 is Count+1,
        max(T,H,Count1,Count,R,C).

 max([H|T],Value,Count,CountV,R,C) :-
        H =< Value,!,
        Count1 is Count+1,
        max(T,Value,Count1,CountV,R,C).       
        
        
%assignPriority(+ListOfMoves,-ListOfPriorities,+ValueOfStack)
assignPriority([],[],1).

assignPriority([H|T],[P|Ps],1):-
        processFirstElement(H,P,1),
        assignPriority(T,Ps,1).

assignPriority([],[],Y):- Y \= 1.

assignPriority([H|T],[P|Ps],Y):-
        processFirstElement(H,P,Y),
        assignPriority(T,Ps,Y).
        
%processFirstElement(+ValueOfAvailableMove,-Priority,+ValueOfStack)
processFirstElement([H|_T],P,1):-
        (H \= 2, 
        H\= 1->
         P is 1;
        (H == 2 ->
         P is 4;!),
        (H == 1 ->
         P is 3;!)).

processFirstElement([H|_T],P,Y):-
        Y \= 1,
         (H == 1 ->
         P is 2;
          P is 1).
        

%advance(+DirectionOfTravel,+NumberOfTravel,+CarryingDisk)
advance(Dir,Nmb,Dsk):-
        write('Direction: \n'),
        write(Dir),
        nl,
        write('Number of movement: \n'),
        write(Nmb),
        nl,
        write('Disk? \n'),
        write(Dsk),
        nl,
        write('Press enter to continue\n'),
        get_char(_).

%printPoints(+PointsOfX,+PointsOfY)
printPoints(PointsX,PointsY) :-
        write('\nPlayer X has '),
        write(PointsX),write(' points'),
        write('\nPlayer Y has '),
        write(PointsY),write(' points'),nl,nl.



%readInput(-Player,-DirectionOfTravel,-NumberOfTravel,-CarryingDisk)
readInput(Pl,Dir,Nmb,Dsk) :- 
        nl, 
        get_char(_),
        write('Player '),write(Pl),write('s turn\n'),
        write('In which direction do you wish to move? (L,D,U or R)\n'),
        get_char(Dir),
        write('How many houses are you traveling? (integer)\n'),
        get_char(_),
        get_code(X),
        Nmb is X-48,
        write('Do you want to carry a disk? (Y or N)\n'),
        get_char(_),
        get_char(Dsk).


%moveDisk(+CarryingDisk,+ValueOfStackWherePlayerIs,+ValueOfStackWherePlayerIsHeaded,-ValueOfStackWherePlayerWas,-ValueOfStackAfterPlayerArrives,+PointsOfPlayer,-NewPointsOfPlayer)
moveDisk('Y',T,Y,X,Z,Points,NewPoints) :- 
        Z is Y+1,
        Z >= 3, 
        X is T-1,
        NewPoints is Points+1.

moveDisk('Y',T,Y,X,Z,Points,Points) :- 
        Z is Y+1,
        Z < 3, 
        X is T-1.

moveDisk('N',T,Y,T,Y,Points,Points).

%validateMove(+LineOfPlayer,+ColumnOfPlayer,-NewLineOfPlayer,-NewColumnOfPlayer,+LineOfEnemy,+ColumnEnemy,+DirectionOfTravel,+NumberOfTravel,+CarryingDisk,+Board,-StackAfterPlayerLeaves,+StackWherePlayerIs,+SymbolOfComplete,+PointsOfPlayer,+NewPointsOfPlayer,+PrevChoiceOfPlayer)
validateMove(Line,Column,NewLine,NewColumn, LineF,ColumnF,Dir,Nmb,Dsk,Brd,NewPrevStack,Stack,StackSymbol,Points,NewPoints,PrevChoice):-
        calcCoord(Dir,Nmb,Line,Column,NewLine,NewColumn),
        getElementM(NewLine,NewColumn,Brd,NextStack),
        isPlayer(NewLine,NewColumn, LineF,ColumnF),!,
        canLand(NextStack,Dsk),!,
        getElementM(Line,Column,Brd,[_H|PrevStack]),
        canDiskMove(Dsk,PrevStack,NextStack,NewPrevStack,NewNextStack,PrevChoice),!,
        isMoveOverPlayer(Dsk,Dir,Line,Column,NewLine,NewColumn,LineF,ColumnF),!,
        isStackComplete(NewNextStack,Stack,StackSymbol,Points,NewPoints),!.

%isPlayer(+LineToTravelTo,+ColumnToTravelTo, +LineOfEnemy,+ColumnOfEnemy)
isPlayer(LineF,ColumnF, LineF,ColumnF):-errorLanding1.
isPlayer(NewLine,NewColumn, LineF,ColumnF):-
        NewLine \= LineF;
        NewColumn \= ColumnF.

%canLand(+ValueOfStackToTravelTo,+CarryingDisk)        
canLand(_Y,'N').
canLand(_Y,'N').
canLand(Y,'Y'):-integer(Y).
canLand(Y,'Y'):-Y == '$',errorLanding2.
canLand(Y,'Y'):-Y == '#',errorLanding2.

        
errorLanding1 :-  write('\n\nYou cannot land on top of another player'),fail.
errorLanding2 :-  write('\n\nYou cannot land on top of a complete stack with a disk'),fail.

%isMoveOverPlayer(+CarryingDisk,+DirectionOfTravel,+LineOfPlayer,+ColumnOfPlayer,+NewLineOfPlayer,+NewColumnOfPlayer,+LineOfEnemy,+ColumnEnemy)
isMoveOverPlayer('N',_Dir,_Line,_Column,_NewLine,_NewColumn,_LineF,_ColumnF).

isMoveOverPlayer('Y',_Dir,Line,Column,_NewLine,_NewColumn,LineF,ColumnF) :-
        Line \= LineF,
        Column \= ColumnF.

isMoveOverPlayer('Y',Dir,_Line,ColumnF,_NewLine,_NewColumn,_LineF,ColumnF) :-
        Dir \= 'U',
        Dir \= 'D'.

isMoveOverPlayer('Y',Dir,LineF,_Column,_NewLine,_NewColumn,LineF,_ColumnF) :-
        Dir \= 'L',
        Dir \= 'R'.

isMoveOverPlayer('Y','U',_Line,ColumnF,NewLine,_NewColumn,LineF,ColumnF) :-
        (NewLine < LineF ->
         errorMoving;
         true).

isMoveOverPlayer('Y','D',_Line,ColumnF,NewLine,_NewColumn,LineF,ColumnF) :-
        (NewLine > LineF ->
         errorMoving;
         true).

isMoveOverPlayer('Y','L',LineF,_Column,_NewLine,NewColumn,LineF,ColumnF) :-
        (NewColumn < ColumnF ->
         errorMoving;
         true).

isMoveOverPlayer('Y','R',LineF,_Column,_NewLine,NewColumn,LineF,ColumnF) :-
        (NewColumn > ColumnF ->
         errorMoving;
         true).

errorMoving :-
        write('\n\nYou cannot move over a player while carrying a disk'), fail.

%canDiskMove(+CarryingDisk,+ValueOfStackWherePlayerIs,+ValueOfStackWherePlayerIsHeaded,-StackAfterPlayerLeaves,-StackAfterPlayerArrives,+PreviousChoice)
canDiskMove('N',PrevStack,NextStack,PrevStack,NextStack,_PrevChoice).


canDiskMove('Y',PrevStack,NextStack,NewPrevStack,NewNextStack,PrevChoice):-
        (PrevChoice \= 'Y'->
        (PrevStack == 1 ->
        (integer(NextStack) ->
         NewNextStack is NextStack +1, NewPrevStack is 0;
         errorDropingDisk);
         errorTakingDisk);
         errorTakingDiskAgain).
        
errorTakingDisk :-
        write('\n\nYou cannot take a disk from a stack unless the disk number in that stack is 1'), fail.

errorDropingDisk :-
         write('\n\nYou cannot drop a disk on a complete stack'), fail.

errorTakingDiskAgain :-
         write('\n\nYou cannot take a disk from the same stack two times in a row'), fail.
                
%isStackComplete(-StackBeingAnalised,+StackOfPlayer,+StackSymbol,+PointsOfPlayer,-NewPointsOfPlayer)
isStackComplete(3,StackSymbol,StackSymbol,Points,NewPoints):-
        NewPoints is Points+1.

isStackComplete(NewStack,NewStack,_StackSymbol,Points,Points).
        
%calcCoord(+DirectionOfTravel,+NumberOfTravel,+LineOfPlayer,+ColumnOfPlayer,-NewLineOfPlayer,-NewColumnOfPlayer)
calcCoord('U',Nmb,Line,Column,NewLine,NewColumn) :-
        NewLine is Line - Nmb,
        NewColumn is Column. 

calcCoord('D',Nmb,Line,Column,NewLine,NewColumn) :-
        NewLine is Line + Nmb,
        NewColumn is Column. 

calcCoord('L',Nmb,Line,Column,NewLine,NewColumn) :-
        NewLine is Line,
        NewColumn is Column-Nmb. 

calcCoord('R',Nmb,Line,Column,NewLine,NewColumn) :-
        NewLine is Line,
        NewColumn is Column+Nmb.

%move(+Player,+LineOfPlayer,+ColumnOfPlayer,+NewLineOfPlayer,+NewColumnOfPlayer,+Board,-ResultingBoard,+StackWherePlayerWas,+StackWherePlayerArrived)
move(Pl,Line,Column,NewLine,NewColumn,Brd,BrdR,NewPrevStack,NewNextStack) :- 
        replaceM(Line,Column,NewPrevStack,Brd,BrdT),
        replaceM(NewLine,NewColumn,[Pl|NewNextStack],BrdT,BrdR).

