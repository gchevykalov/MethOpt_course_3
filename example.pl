% Предполагаем, что x из S.
belong(x,s,fact).

% Предполагаем, что y из co(S).
belong(y,co,fact).

% Предполагаем, что z из U.
belong(z,u,fact).

% Правило, позволяющее проверить, принадлежит ли X множеству S.
belong(X,s,rule):-
   (   belong(X,s,fact),!,
                     % ^ Выполняем отсечение, поскольку достаточно просто проверить, есть ли такой факт. (В обычном случае
                     % belong(X,s,fact) возвращает сначала true, если факт есть, затем продолжает возвращать true
                     % столько раз, сколько такой факт определен. В конце всегда возвращается false. Если такого факта нет,
                     % просто возвращается false).
       write(X),
       write(' принадлежит множеству s по предположению.\n')   );
   (   write('Проверим значения координат '),
       write(X),
       write('.\n'),
       discrete_coords_values(X,List), % Получаем список возможных значений координат X и проверяем по определению S.
       length(List,List_length),
       List_length == 2,
       List = [Head|Tail],
       abs(Head,Abs_head),
       Abs_head == 1,
       Tail = [Tail_head],
       abs(Tail_head,Abs_tail),
       Abs_tail == 1,
       write(X),
       write(' принадлежит множеству s по определению s.\n')   ).

% Правило, позволяющее проверить, принадлежит ли X множеству U.
belong(X,u,rule):-
    (   belong(X,u,fact),!,
                       % ^ Выполняем отсечение, поскольку достаточно просто проверить, есть ли такой факт.
        write(X),
        write(' принадлежит множеству u по предположению.\n')   );
    (   write('Оценим значения координат '),
        write(X),
        write('.\n'),
        coords_values(X,Max,Min), % Получаем интервал значений координат X и проверяем по определению U.
        Max =< 1,
        Min >= -1,
        write(X),
        write(' принадлежит множеству u по определению u.\n')   ).

% Правило, позволяющее проверить, принадлежит ли X множеству co(S).
belong(X,co,rule):-
    (   belong(X,co,fact),!,
                        % ^ Выполняем отсечение, поскольку достаточно просто проверить, есть ли такой факт.
        write(X),
        write(' принадлежит множеству co(s) по предположению.\n')   );
    (   write('Проверим можно ли представить '),
        write(X),
        write(' в виде выпуклой комбинации элементов из s.\n'),
        convex_combination(X,s), % Проверяем, можно ли представить X как выпуклую комбинацию элементов из S.
        write(X),
        write(' принадлежит множеству co(s) по определению co(s).\n')   ).

% Правило позволяющее проверить совпадение множеств A и B.
% Именно проверка этого утверждения и является нашей целью.
equals(A,B):-
    (   write('Возьмем '),
        belong(X,A,fact),!,
                       % ^ Выполняем отсечение, поскольку достаточно выбрать один x.
        write(X),
        write(' из '),
        write(A),
        write(' и проверим его принадлежность множеству '),
        write(B),
        write('.\n'),
        belong(X,B,rule),
        write('Установлено включение '),
        write(A),
        write(' в '),
        write(B),
        write('.\n\n'),
        write('Теперь возьмем '),
        belong(Y,B,fact),!,
                       % ^ Выполняем отсечение, поскольку достаточно выбрать один y.
        write(Y),
        write(' из '),
        write(B),
        write(' и проверим его принадлежность множеству '),
        write(A),
        write('.\n'),
        belong(Y,A,rule),
        write('Установлено включение '),
        write(B),
        write(' в '),
        write(A),
        write('.\n')   ).

% Набор возможных значений координат x задаем промежутком.
% (Дискретность значений, в случае X из S, не меняет способа оценки для
% дальнейших рассуждений). Если X не из S, U или co(S), то сказать
% изначально ничего не можем.
coords_values(X,Max,Min):-
    (   ((   belong(X,s,fact),
             write('Оцениваем значения координат из определения s.\n')   );
         (   belong(X,u,fact),
             write('Оцениваем значения координат из определения u.\n')   )),!,
                                                                          % ^ Выполняем отсечение, поскольку достаточно
                                                                          % просто проверить, есть ли такой факт.
        Max is 1,   % Значения задаются из определений S и U.
        Min is -1   );
    (   belong(X,co,fact),!,
                        % ^ Выполняем отсечение, поскольку достаточно просто проверить, есть ли такой факт.
        write('Оцениваем значения координат из определения co(s).\n'),
        combination_values(Comb_max,Comb_min,lambda,s),
        Max is Comb_max,  % Значения задаются из определения co(S).
        Min is Comb_min   ).

% Список возможных значений координат X (применимо только если возможных
% значений конечное число).
discrete_coords_values(X,List):-
    (   belong(X,s,fact),!,
                       % ^ Выполняем отсечение, поскольку достаточно просто проверить, есть ли такой факт.
        write('Получаем список возможных значений координат из определения s.\n'),
        List = [-1, 1]   ).

% Все лямбды больше либо равны 0.
not_negative(lambda).

% Сумма всех лямбд равна 1.
sum_all(lambda,Sum):-
    Sum is 1.

% В случае, если все элементы суммы положительны, максимальная частичная
% сумма не превосходит всей суммы. В противном случае, при
% неопределенных слагаемых сказать ничего нельзя.
upper_sum_part(X,Sum):-
    (   not_negative(X),
        write('Воспользуемся неотрицательностью '),
        write(X),
        write(', чтобы оценить наибольшее значение части суммы.\n'),
        sum_all(X,Sum_all),
        Sum is Sum_all   ).

% В случае, если все элементы суммы положительны, минимальная частичная
% сумма не меньше 0. В противном случае, при неопределенных слагаемых
% сказать ничего нельзя.
lower_sum_part(X,Sum):-
    (   not_negative(X),
        write('Воспользуемся неотрицательностью '),
        write(X),
        write(', чтобы оценить наименьшее значение части суммы.\n'),
        Sum is 0   ).

% Проверяет можно ли представить элемент X выпуклой комбинацией
% элементов из B. Применимо только для B с одинаковыми ограничениями на
% координаты.
convex_combination(X,B):-
    (   belong(Y,B,fact),!,
                       % ^ Выполняем отсечение, поскольку достаточно выбрать один y.
        coords_values(Y,Y_max,Y_min),
        coords_values(X,X_max,X_min),
        try_build_convex_combination(X_max,X_min,Y_max,Y_min)   ).

% Оценка значений линейной комбинации sum(Coefs[i]*x[i]), где x из A.
combination_values(Max,Min,Coefs,A):-
    (   belong(X,A,fact),!,
                       % ^ Выполняем отсечение, поскольку достаточно выбрать один x.
        upper_sum_part(Coefs,Upper_sum),
        lower_sum_part(Coefs,Lower_sum),
        coords_values(X,Coords_max,Coords_min),
        List = [Upper_sum * Coords_max,Upper_sum * Coords_min,
               Lower_sum * Coords_max,Lower_sum * Coords_min],
        max_list(List,Max),
        min_list(List,Min)   ).

% Пытаемся строить выпуклую комбинацию на основе интервалов значений
% координат.
try_build_convex_combination(X_max,X_min,Y_max,Y_min):-
    (   Y_max >= X_max,
        Y_min =< X_min,
        combinate_by_num_coord('n',X_max,X_min,Y_max,Y_min),!   ).
                                                          % ^ Выполняем отсечение, поскольку не нужно проверять
                                                          % все ветвления внутри индукции.

% Метод индукции для проверки представления элемента выпуклой
% комбинацией, опирается на интервалы значений.
combinate_by_num_coord(Num,X_max,X_min,Y_max,Y_min):-
    (   Num == 'm-1',
        write('Для m-1 делаем предположение.\n')   );
    (   Num == '1',
        write('Проверим для 1.\n'),
        evaluate_coef(X_max,X_min,Y_max,Y_min,Min,Max,Sum),
        Min >= 0,
        Max =< 1,
        Sum == 1   );
    (   Num == 'm',
        write('Воспользуемся методом индукции: на каждом шаге пытаемся получить на одну координату больше.\n'),
        combinate_by_num_coord('1',X_max,X_min,Y_max,Y_min),
        combinate_by_num_coord('m-1',X_max,X_min,Y_max,Y_min),
        write('Сделаем переход от m-1 к m.\n'),
        evaluate_coefs_in_comb_of_comb(X_max,X_min,Y_max,Y_min,0,1,Comb_min,Comb_sum),
        Comb_min >= 0,
        Comb_sum == 1   );
    (   combinate_by_num_coord('m',X_max,X_min,Y_max,Y_min),
        write('Поскольку методом индукции показано, что верно для любого m, то верно и для '),
        write(Num),
        write('.\n')   ).

% Оценка коэффициентов в комбинации.
evaluate_coef(X_max,X_min,Y_max,Y_min,Min,Max,Sum):-
    (   write('Воспользуемся представлением t=(1-alpha)*t_1+alpha*t_2, где aplha определяется из условия: (1-alpha)*a+alpha*b=x. Это позволяет выполнить текущее условие, не нарушая старых.\n'),
        Sum is 1,
        List = [(Y_max - X_max)/(Y_max - Y_min),(X_max - Y_min)/(Y_max - Y_min),
               (Y_max - X_min)/(Y_max - Y_min),(X_min - Y_min)/(Y_max - Y_min)],
        max_list(List,Max),
        min_list(List,Min)   ).

% Оценка коэициентов в комбинации комбинаций.
evaluate_coefs_in_comb_of_comb(X_max,X_min,Y_max,Y_min,Out_min,Out_sum,Comb_min,Comb_sum):-
    (   write('Составим выпуклую комбинацию выпуклых комбинаций. При этом будем во внутренних комбинациях выбирать элементы, отличающиеся только в одной координате.\n'),
        evaluate_coef(X_max,X_min,Y_max,Y_min,Min,Max,Sum),
        Comb_sum is Sum * Out_sum,
        (Out_min < 0,
        Comb_min is Out_min * Max;
        Comb_min is Out_min * Min)   ).
