insert into
full_orders (`client_name`, `client_brth`, `client_pass_id`,`nomber_race`, `airport_out`, `airport_in`, `date_depart`, `date_arrive`)
values ('$gen1'
        , '$gen2'
        , '$gen3'
        , (select ticket.race_nomber from ticket where idTicket = '$gen4')
        , (select ticket.airport_out from ticket where idTicket = '$gen4')
        , (select ticket.airport_in from ticket where idTicket = '$gen4')
        , (select ticket.date_depart from ticket where idTicket = '$gen4')
        , (select ticket.date_arrive from ticket where idTicket = '$gen4'));
