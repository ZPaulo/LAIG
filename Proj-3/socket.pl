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
        write(Stream),nl,
       
        serverLoop(Stream),
        socket_server_close(Socket).

serverLoop(Stream) :-
        repeat,
        read(Stream, ClientMsg),
        write('Received: '), write(ClientMsg), nl,
        parse_input(ClientMsg,Stream),
       
        (ClientMsg == quit; ClientMsg == end_of_file), !.


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%                   Stack                   %%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


parse_input(validate,Stream) :-
        read(Stream,Line),write(Line),nl,
        read(Stream,Column),write(Column),nl,
        read(Stream,NewLine),write(NewLine),nl,
        read(Stream,NewColumn),write(NewColumn),nl,
        read(Stream,LineE),write(LineE),nl,
        read(Stream,ColumnE),write(ColumnE),nl,
        read(Stream,Dir),write(Dir),nl,
        read(Stream,Nmb),write(Nmb),nl,
        read(Stream,Dsk),write(Dsk),nl,
        read(Stream,Brd),write(Brd),nl,
        read(Stream,SS),atom_codes(Symbol,SS),write(Symbol),nl,
        read(Stream,Points),write(Points),nl,
        read(Stream,PrevChoice),write(PrevChoice),nl,
        
        validateMove(Line,Column,_,_, LineE,ColumnE,Dir,Nmb,Dsk,Brd,_,_,Symbol,Points,NewPoints,PrevChoice,Stream)->
                (write(Stream,'yes\n'), flush_output(Stream), format(Stream,'~q~n',[NewPoints]),flush_output(Stream));
                (write(Stream,'no\n'),  flush_output(Stream)).

parse_input(move(Pl,Line,Column,NewLine,NewColumn,Brd,BrdR,NewPrevStack,NewNextStack),Stream):-
        move(Pl,Line,Column,NewLine,NewColumn,Brd,BrdR,NewPrevStack,NewNextStack)->
                (write(Stream,'yes\n'), flush_output(Stream), format(Stream,'~q~n',[BrdR]),flush_output(Stream));
                (write(Stream,'no\n'),  flush_output(Stream)).

parse_input(random,Stream):-
        read(Stream,Max),write(Max),nl,
        read(Stream,Line),write(Line),nl,
        read(Stream,Column),write(Column),nl,
        read(Stream,LineE),write(LineE),nl,
        read(Stream,ColumnE),write(ColumnE),nl,
        read(Stream,Brd),write(Brd),nl,
        read(Stream,SS),atom_codes(Symbol,SS),write(Symbol),nl,
        read(Stream,Points),write(Points),nl,
        read(Stream,PrevChoice),write(PrevChoice),nl,
        gameLogic(Symbol,Line,Column,LineE,ColumnE,Brd,Points,PrevChoice,[Max,Max|[]],random,Stream).

parse_input(smart,Stream):- 
        read(Stream,Max),write(Max),nl,
        read(Stream,Line),write(Line),nl,
        read(Stream,Column),write(Column),nl,
        read(Stream,LineE),write(LineE),nl,
        read(Stream,ColumnE),write(ColumnE),nl,
        read(Stream,Brd),write(Brd),nl,
        read(Stream,SS),atom_codes(Symbol,SS),write(Symbol),nl,
        read(Stream,Points),write(Points),nl,
        read(Stream,PrevChoice),write(PrevChoice),nl,     
        gameLogic(Symbol,Line,Column,LineE,ColumnE,Brd,Points,PrevChoice,[Max,Max|[]],smart,Stream).
        


start(Stream):- 
        
        
        createBoardInput(Lines,Columns,Stream),
        createBoard(Brd,Lines,Columns),
        
        
        selectGameModeInput(Mode,AIInt,Stream),
        printBoard(Brd),
        format(Stream, '~q.~n', [Brd]),
        flush_output(Stream),
        write('Press enter to start'),
        write(Stream,'Press enter to start\n'),
         flush_output(Stream),
        ((Mode == ai-ai;Mode == ai-h;Mode == h-ai)->
         get_char(Stream,_);!),
        Column is Columns-1,
        Line is Lines-1, 
        game('X',0,Column,Line,0,Brd,0,0,n,n,Mode,[Lines,Columns|[]],AIInt,Stream).



%selectGameModeInput(-PlayerVsPlayer,-LevelOfIntelligence)
selectGameModeInput(Mode,AIInt,Stream):-
        write('In which mode do you want to play?(ex: h-h or ai-h)\n'),
        write(Stream,'In which mode do you want to play?(ex: h-h or ai-h)\n'),
        flush_output(Stream),
      
        read(Stream,Mode),
        print(Mode),nl,
       % name(Mode),
         write('How should AI be?(random or smart)\n'),
         write(Stream,'How should AI be?(random or smart)\n'),
         flush_output(Stream),
         read(Stream,AIInt),
         %name(AIInt,Int),
         write(AIInt),nl.


%createBoardInput(-Number of Lines,-Number of Columns)
createBoardInput(Lines,Columns,Stream):-
       
        write('How many lines will the board have?\n'),
        write(Stream,'How many lines will the board have?\n'),
        flush_output(Stream),
        read(Stream,Lines),
        
        write('How many columns will the board have?\n'),
        write(Stream,'How many columns will the board have?\n'),
       flush_output(Stream),
        read(Stream,Columns),
        
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
gameOver(Pl,Stream) :-
        nl,
        write('Player '), write(Pl), write(' has won!!!'),
         write(Stream,'Winner\n'),
          flush_output(Stream),
          format(Stream, '~q.~n', [Pl]),
          flush_output(Stream).

%game(+Player,+LineOfX,+ColumnOfX,+LineOfY,+ColumnOfY,+Board,+PointsOfX,+PointsOfY,+PrevChoiceOfX,+PrevChoiceOfY,+Mode,+BoardSize,+AILevel)
game('X',LineX,ColumnX,LineY,ColumnY,Brd,PointsX,PointsY,PrevChoiceX,PrevChoiceY,Mode,BrdSize,AILevel,Stream):-
        PointsY < 4,
        printPoints(PointsX,PointsY,Stream),
        (Mode == h-h->
         gameLogic('X','$',LineX,ColumnX,NewLine,NewColumn,LineY,ColumnY,Brd,BrdR,PointsX,NewPoints,PrevChoiceX,h,BrdSize,AILevel,Stream),!,
         game('Y',NewLine,NewColumn,LineY,ColumnY,BrdR,NewPoints,PointsY,PrevChoiceX,PrevChoiceY,Mode,BrdSize,AILevel,Stream);!),
        (Mode == h-ai->
         gameLogic('X','$',LineX,ColumnX,NewLine,NewColumn,LineY,ColumnY,Brd,BrdR,PointsX,NewPoints,PrevChoiceX,h,BrdSize,AILevel,Stream),!,
         game('Y',NewLine,NewColumn,LineY,ColumnY,BrdR,NewPoints,PointsY,PrevChoiceX,PrevChoiceY,ai-h,BrdSize,AILevel,Stream);!),
        (Mode == ai-h->
         gameLogic('X','$',LineX,ColumnX,NewLine,NewColumn,LineY,ColumnY,Brd,BrdR,PointsX,NewPoints,PrevChoiceX,ai,BrdSize,AILevel,Stream),!,
         game('Y',NewLine,NewColumn,LineY,ColumnY,BrdR,NewPoints,PointsY,PrevChoiceX,PrevChoiceY,h-ai,BrdSize,AILevel,Stream);!),
        (Mode == ai-ai->
         gameLogic('X','$',LineX,ColumnX,NewLine,NewColumn,LineY,ColumnY,Brd,BrdR,PointsX,NewPoints,PrevChoiceX,ai,BrdSize,AILevel,Stream),!,
         game('Y',NewLine,NewColumn,LineY,ColumnY,BrdR,NewPoints,PointsY,PrevChoiceX,PrevChoiceY,Mode,BrdSize,AILevel,Stream);!).

game('Y',LineX,ColumnX,LineY,ColumnY,Brd,PointsX,PointsY,PrevChoiceX,PrevChoiceY,Mode,BrdSize,AILevel,Stream):-
        PointsX < 4,
        printPoints(PointsX,PointsY,Stream),
        (Mode == h-h->
         gameLogic('Y','#',LineY,ColumnY,NewLine,NewColumn,LineX,ColumnX,Brd,BrdR,PointsY,NewPoints,PrevChoiceY,h,BrdSize,AILevel,Stream),!,
         game('X',LineX,ColumnX,NewLine,NewColumn,BrdR,PointsX,NewPoints,PrevChoiceX,PrevChoiceY,Mode,BrdSize,AILevel,Stream);!),
        (Mode == h-ai->
         gameLogic('Y','#',LineY,ColumnY,NewLine,NewColumn,LineX,ColumnX,Brd,BrdR,PointsY,NewPoints,PrevChoiceY,h,BrdSize,AILevel,Stream),!,
         game('X',LineX,ColumnX,NewLine,NewColumn,BrdR,PointsX,NewPoints,PrevChoiceX,PrevChoiceY,ai-h,BrdSize,AILevel,Stream);!),
        (Mode == ai-h->
         gameLogic('Y','#',LineY,ColumnY,NewLine,NewColumn,LineX,ColumnX,Brd,BrdR,PointsY,NewPoints,PrevChoiceY,ai,BrdSize,AILevel,Stream),!,
         game('X',LineX,ColumnX,NewLine,NewColumn,BrdR,PointsX,NewPoints,PrevChoiceX,PrevChoiceY,h-ai,BrdSize,AILevel,Stream);!),
        (Mode == ai-ai->
         gameLogic('Y','#',LineY,ColumnY,NewLine,NewColumn,LineX,ColumnX,Brd,BrdR,PointsY,NewPoints,PrevChoiceY,ai,BrdSize,AILevel,Stream),!,
         game('X',LineX,ColumnX,NewLine,NewColumn,BrdR,PointsX,NewPoints,PrevChoiceX,PrevChoiceY,Mode,BrdSize,AILevel,Stream);!).

        
game('X',_LineX,_ColumnX,_LineY,_ColumnY,_Brd,_PointsX,4,_PrevChoiceX,_PrevChoiceY,_Mode,_BrdSize,_AILevel,Stream):-
        gameOver('Y',Stream).

game('Y',_LineX,_ColumnX,_LineY,_ColumnY,_Brd,4,_PointsY,_PrevChoiceX,_PrevChoiceY,_Mode,_BrdSize,_AILevel,Stream):-
        gameOver('X',Stream).

%gameLogic(+Player,+SymbolOfComplete,+LineOfPlayer,+ColumnOfPlayer,-NewLineOfPlayer,-NewColumnOfPlayer,+LineOfEnemy,+ColumnEnemy,+Board,-ResultingBoard,+PointsOfPlayer,-NewPointsOfPlayer,+PrevChoiceOfPlayer,+TypeOfPlayer,+BoardSize,+AILevel)
gameLogic(Pl,Symbol,Line,Column,NewLine,NewColumn,LineE,ColumnE,Brd,BrdR,Points,NewPoints,PrevChoice,h,BrdSize,AILevel,Stream):-
        readInput(Pl,Dir,Nmb,Dsk,Stream),!,
        (validateMove(Line,Column,NewLine,NewColumn, LineE,ColumnE,Dir,Nmb,Dsk,Brd,NewPrevStack,NewNextStack,Symbol,Points,NewPoints,PrevChoice,Stream)->
         write(Stream,'Move Accepted\n'),
        move(Pl,Line,Column,NewLine,NewColumn,Brd,BrdR,NewPrevStack,NewNextStack),
        printBoard(BrdR);
        format(Stream, '~q.~n', [BrdR]),
        flush_output(Stream),
        gameLogic(Pl,Symbol,Line,Column,NewLine,NewColumn,LineE,ColumnE,Brd,BrdR,Points,NewPoints,PrevChoice,h,BrdSize,AILevel,Stream)).

gameLogic(Symbol,Line,Column,LineE,ColumnE,Brd,Points,PrevChoice,[MaxLine,MaxColumn|[]],random,Stream):-
        generateRandomInput(Dir,Nmb,Dsk,MaxColumn,MaxLine),!,
        (validateMove(Line,Column,_,_, LineE,ColumnE,Dir,Nmb,Dsk,Brd,_,_,Symbol,Points,NewPoints,PrevChoice,Stream)->
        write(Stream,'Move Accepted\n'),
        flush_output(Stream),
        format(Stream,'~q~n',[NewPoints]),
        flush_output(Stream),
        advance(Dir,Nmb,Dsk,Stream); 
        gameLogic(Symbol,Line,Column,LineE,ColumnE,Brd,Points,PrevChoice,[MaxLine,MaxColumn|[]],random,Stream)).

gameLogic(Symbol,Line,Column,LineE,ColumnE,Brd,Points,PrevChoice,[MaxLine,MaxColumn|[]],smart,Stream):-
        getAvailableMoves(Moves,Brd,Line,Column,MaxLine,MaxColumn),
        getElementM(Line,Column,Brd,[_H|PrevStack]),
        chooseValidPlay(Moves,Nmb,Dsk,Dir,Brd,PrevStack,Line,Column,_,_,LineE,ColumnE,_,_,Symbol,Points,NewPoints,PrevChoice,Stream),
        write(Stream,'Move Accepted\n'),
        flush_output(Stream),
        format(Stream,'~q~n',[NewPoints]),
        flush_output(Stream),
        advance(Dir,Nmb,Dsk,Stream).




%chooseValidPlay(+AvailableMoves,-NumberOfTravel,-CarryingDisk,-Direction,+Board,+StackWherePlayerIs,+LineOfPlayer,+ColumnOfPlayer,-NewLineOfPlayer,-NewColumnOfPlayer,+LineOfEnemy,+ColumnEnemy,-StackAfterPlayerLeaves,-StackAfterPlayerArrives,+SymbolOfComplete,+PointsOfPlayer,+NewPointsOfPlayer,+PrevChoiceOfPlayer)
chooseValidPlay(Moves,Nmb,Dsk,Dir,Brd,PrevStack,Line,Column,NewLine,NewColumn,LineF,ColumnF,NewPrevStack,NewNextStack,Symbol,Points,NewPoints,PrevChoice,Stream):-
        decidePlay(Moves,Play,PrevStack),
        processPlay(Play,Nmb,Dsk,Dir,Line,Column),
        (validateMove(Line,Column,NewLine,NewColumn, LineF,ColumnF,Dir,Nmb,Dsk,Brd,NewPrevStack,NewNextStack,Symbol,Points,NewPoints,PrevChoice,Stream)->
         !;
         deleteInvalid(Moves,Play,MovesR),
          chooseValidPlay(MovesR,_Nmb,_Dsk,_Dir,Brd,PrevStack,Line,Column,NewLine,NewColumn,LineF,ColumnF,NewPrevStack,NewNextStack,Symbol,Points,NewPoints,PrevChoice,Stream)).

%deleteInvalid(+AvailableMoves,+PlayToDelete,-ResultingListOfMoves)
deleteInvalid(Moves,[H|_T],MovesR):-
        delete(Moves,H,MovesR).

%generateRandomInput(-DirectionOfTravel,-NumberOfTravel,-CarryingDisk,+NumberOfColumnsInBoard,+NumberOfLinesInBoard)
generateRandomInput(Dir,Nmb,Dsk,MaxColumn,MaxLine):-
        random_select(Dir, [u,d,l,r], _Rest1),
        random_select(Dsk, [y,n],_Rest),
        ((Dir == u;Dir == d)->
        random(1,MaxLine,Nmb);
        random(1,MaxColumn,Nmb)).

%processPlay(+Play,-NumberOfTravel,-CarryingDisk,-DirectionOfTravel,+LineOfPlayer,+ColumnOfPlayer)
processPlay([Play,Pri],Nmb,y,Dir,Line,Column):-
        Pri > 2,!,
        calculateMove(Play,Nmb,Dir,Line,Column). 
        
processPlay([Play,Pri],Nmb,n,Dir,Line,Column):-
        Pri =< 2,!,
        calculateMove(Play,Nmb,Dir,Line,Column).
        
%calculateMove(+Play,-NumberOfTravel,-DirectionOfTravel,+LineOfPlayer,+ColumnOfPlayer)
calculateMove([_Value,LineF,Column|_T],Nmb,r,LineF,ColumnF):-
              Column - ColumnF > 0,
              Nmb is Column - ColumnF.

calculateMove([_Value,Line,ColumnF|_T],Nmb,d,LineF,ColumnF):-
              Line - LineF > 0,
              Nmb is Line - LineF.

calculateMove([_Value,Line,ColumnF|_T],Nmb,u,LineF,ColumnF):-
              LineF - Line > 0,
              Nmb is LineF - Line.

calculateMove([_Value,LineF,Column|_T],Nmb,l,LineF,ColumnF):-
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
advance(Dir,Nmb,Dsk,Stream):-
        write(Stream,'Direction\n'),
        flush_output(Stream),
        format(Stream, '~q.~n', [Dir]),
        flush_output(Stream),
        
        write(Stream,'Number of movement\n'),
        flush_output(Stream),
        format(Stream, '~q.~n', [Nmb]),
        flush_output(Stream),
        
        write(Stream,'Disk?\n'),
        flush_output(Stream),
        format(Stream, '~q.~n', [Dsk]),
        flush_output(Stream),
        
        format(Stream, '~q.~n', ['Press enter to continue']),
        flush_output(Stream),
        get_char(Stream,_).

%printPoints(+PointsOfX,+PointsOfY)
printPoints(PointsX,PointsY,Stream) :-
        write('\nPlayer X has '),
        write(PointsX),write(' points'),
        write('\nPlayer Y has '),
        write(PointsY),write(' points'),nl,nl,
        
        write(Stream,'Points\n'),
        flush_output(Stream),
        format(Stream, '~q.~n', [PointsX]),
        flush_output(Stream),
        format(Stream, '~q.~n', [PointsY]),
        flush_output(Stream).



%readInput(-Player,-DirectionOfTravel,-NumberOfTravel,-CarryingDisk)
readInput(Pl,Dir,Nmb,Dsk,Stream) :- 
        nl, 
      
        write('Player '),write(Pl),write('s turn\n'),
        write(Stream,'Player Turn\n'),
        flush_output(Stream),
        format(Stream, '~q.~n', [Pl]),
        flush_output(Stream),
        
        write('In which direction do you wish to move? (l,d,u or r)\n'),
        format(Stream, '~q.~n', ['Direction: (l,d,u or r)']),
        flush_output(Stream),
        get_char(Stream,Dir),
        
        write('How many houses are you traveling? (integer)\n'),
        format(Stream, '~q.~n', ['How many houses are you traveling? (integer)']),
        flush_output(Stream),
        get_code(Stream,X),
        
        Nmb is X-48,
        write('Do you want to carry a disk? (y or n)\n'),
        format(Stream, '~q.~n', ['Do you want to carry a disk? (y or n)']),
        flush_output(Stream),
        get_char(Stream,Dsk).


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
validateMove(Line,Column,NewLine,NewColumn, LineF,ColumnF,Dir,Nmb,Dsk,Brd,NewPrevStack,Stack,StackSymbol,Points,NewPoints,PrevChoice,Stream):-
        calcCoord(Dir,Nmb,Line,Column,NewLine,NewColumn),
        getElementM(NewLine,NewColumn,Brd,NextStack),
        isPlayer(NewLine,NewColumn, LineF,ColumnF,Stream),!,
        canLand(NextStack,Dsk,Stream),!,
        getElementM(Line,Column,Brd,[_H|PrevStack]),
        canDiskMove(Dsk,PrevStack,NextStack,NewPrevStack,NewNextStack,PrevChoice,Stream),!,
        isMoveOverPlayer(Dsk,Dir,Line,Column,NewLine,NewColumn,LineF,ColumnF,Stream),!,
        isStackComplete(NewNextStack,Stack,StackSymbol,Points,NewPoints),!.

%isPlayer(+LineToTravelTo,+ColumnToTravelTo, +LineOfEnemy,+ColumnOfEnemy)
isPlayer(LineF,ColumnF, LineF,ColumnF,Stream):-errorLanding1(Stream).
isPlayer(NewLine,NewColumn, LineF,ColumnF,_):-
        NewLine \= LineF;
        NewColumn \= ColumnF.

%canLand(+ValueOfStackToTravelTo,+CarryingDisk)        
canLand(_Y,n,_).
canLand(_Y,n,_).
canLand(Y,y,_):-integer(Y).
canLand(Y,y,Stream):-Y == '$',errorLanding2(Stream).
canLand(Y,y,Stream):-Y == '#',errorLanding2(Stream).

        
errorLanding1(Stream) :-  format(Stream, '~q.~n', ['You cannot land on top of another player']),flush_output(Stream),fail.
errorLanding2(Stream) :-  format(Stream, '~q.~n', ['You cannot land on top of a complete stack with a disk']),flush_output(Stream),fail.

%isMoveOverPlayer(+CarryingDisk,+DirectionOfTravel,+LineOfPlayer,+ColumnOfPlayer,+NewLineOfPlayer,+NewColumnOfPlayer,+LineOfEnemy,+ColumnEnemy)
isMoveOverPlayer(n,_Dir,_Line,_Column,_NewLine,_NewColumn,_LineF,_ColumnF,_).

isMoveOverPlayer(y,_Dir,Line,Column,_NewLine,_NewColumn,LineF,ColumnF,_) :-
        Line \= LineF,
        Column \= ColumnF.

isMoveOverPlayer(y,Dir,_Line,ColumnF,_NewLine,_NewColumn,_LineF,ColumnF,_) :-
        Dir \= 'u',
        Dir \= 'd'.

isMoveOverPlayer(y,Dir,LineF,_Column,_NewLine,_NewColumn,LineF,_ColumnF,_) :-
        Dir \= l,
        Dir \= r.

isMoveOverPlayer(y,u,_Line,ColumnF,NewLine,_NewColumn,LineF,ColumnF,Stream) :-
        (NewLine < LineF ->
         errorMoving(Stream);
         true).

isMoveOverPlayer(y,d,_Line,ColumnF,NewLine,_NewColumn,LineF,ColumnF,Stream) :-
        (NewLine > LineF ->
         errorMoving(Stream);
         true).

isMoveOverPlayer(y,l,LineF,_Column,_NewLine,NewColumn,LineF,ColumnF,Stream) :-
        (NewColumn < ColumnF ->
         errorMoving(Stream);
         true).

isMoveOverPlayer(y,r,LineF,_Column,_NewLine,NewColumn,LineF,ColumnF,Stream) :-
        (NewColumn > ColumnF ->
         errorMoving(Stream);
         true).

errorMoving(Stream) :-
        format(Stream, '~q.~n', ['You cannot move over a player while carrying a disk']),flush_output(Stream), fail.

%canDiskMove(+CarryingDisk,+ValueOfStackWherePlayerIs,+ValueOfStackWherePlayerIsHeaded,-StackAfterPlayerLeaves,-StackAfterPlayerArrives,+PreviousChoice)
canDiskMove(n,PrevStack,NextStack,PrevStack,NextStack,_PrevChoice,_).


canDiskMove(y,PrevStack,NextStack,NewPrevStack,NewNextStack,PrevChoice,Stream):-
        (PrevChoice \= y->
        (PrevStack == 1 ->
        (integer(NextStack) ->
         NewNextStack is NextStack +1, NewPrevStack is 0;
         errorDropingDisk(Stream));
         errorTakingDisk(Stream));
         errorTakingDiskAgain(Stream)).
        
errorTakingDisk(Stream) :-
        format(Stream, '~q.~n', ['You cannot take a disk from a stack unless the disk number in that stack is 1']),flush_output(Stream), fail.

errorDropingDisk(Stream) :-
         format(Stream, '~q.~n', ['You cannot drop a disk on a complete stack']),flush_output(Stream), fail.

errorTakingDiskAgain(Stream) :-
         format(Stream, '~q.~n', ['nYou cannot take a disk from the same stack two times in a row']),flush_output(Stream), fail.
                
%isStackComplete(-StackBeingAnalised,+StackOfPlayer,+StackSymbol,+PointsOfPlayer,-NewPointsOfPlayer)
isStackComplete(3,StackSymbol,StackSymbol,Points,NewPoints):-
        NewPoints is Points+1.

isStackComplete(NewStack,NewStack,_StackSymbol,Points,Points).
        
%calcCoord(+DirectionOfTravel,+NumberOfTravel,+LineOfPlayer,+ColumnOfPlayer,-NewLineOfPlayer,-NewColumnOfPlayer)
calcCoord(u,Nmb,Line,Column,NewLine,NewColumn) :-
        NewLine is Line - Nmb,
        NewColumn is Column. 

calcCoord(d,Nmb,Line,Column,NewLine,NewColumn) :-
        NewLine is Line + Nmb,
        NewColumn is Column. 

calcCoord(l,Nmb,Line,Column,NewLine,NewColumn) :-
        NewLine is Line,
        NewColumn is Column-Nmb. 

calcCoord(r,Nmb,Line,Column,NewLine,NewColumn) :-
        NewLine is Line,
        NewColumn is Column+Nmb.

%move(+Player,+LineOfPlayer,+ColumnOfPlayer,+NewLineOfPlayer,+NewColumnOfPlayer,+Board,-ResultingBoard,+StackWherePlayerWas,+StackWherePlayerArrived)
move(Pl,Line,Column,NewLine,NewColumn,Brd,BrdR,NewPrevStack,NewNextStack) :- 
        replaceM(Line,Column,NewPrevStack,Brd,BrdT),
        replaceM(NewLine,NewColumn,[Pl|NewNextStack],BrdT,BrdR).

