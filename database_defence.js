//npm install --save mysql

var mysql = require('mysql');
var http = require('http');
var url = require('url');

var server = http.createServer();
var connection = mysql.createConnection( {
  host      : 'localhost',
  user      : 'root',
  password  : '34862365',
  database  : 'defence'
});

connection.connect();

server.listen(3000, function(){
    console.log('Start Server Listen...');
});

server.on('request', function(req, res){
    console.log('on request');
    if( req.method != 'POST' ){
        return;
    }

    req.on('data', function(chunk){
        console.log('on data ' + chunk.toString());

        var data = chunk.toString().split('/');

        if (data[0] == 'hero') {
          var sql = 'select * from hero where type = ? and count = ?';

          connection.query(sql, [data[1], data[2]], function(err, row, fields) {
            res.end('hero/'+'/'+row[0].sprite+'/'+row[0].name+'/'+row[0].type+'/'+row[0].count+'/'+row[0].speed+'/'+row[0].atk_range+'/'+row[0].damage);
          });
        }
        else if (data[0] == 'help_hero') {
          var sql = 'select * from hero where type = ? and count = ?';

          connection.query(sql, [data[1], data[2]], function(err, row, fields) {
            res.end('help_hero/'+row[0].sprite+'/'+row[0].name+'/'+row[0].type+'/'+row[0].count+'/'+row[0].speed+'/'+row[0].atk_range+'/'+row[0].damage);
          });
        }
        else if(data[0] == 'monster') {
          var sql = 'select * from monster_info where stage = ?';

          connection.query(sql, [data[1]], function(err, row, fields) {
            res.end('monster/'+row[0].hp+'/'+row[0].def);
          });
        }
        else if (data[0] == 'time') {
          var sql = 'update room_list set time = ?, stage = ? where id = ?';

          connection.query(sql, [data[1], data[2], data[3]], function() {
            console.log('success time');
            res.end('');
          });
        }
        else if (data[0] == 'mix_hero') {
          var temp = "mix_hero/";
          var sql = 'select count(*) as c from mix_hero_result where num = ?';

          connection.query(sql, [data[1]], function(err1, row1, fields1) {
            if (row1[0].c > 0) {
              sql = 'select * from mix_hero_result where num = ?';

              connection.query(sql, [data[1]], function(err2, row2, fields2) {
                temp += row2[0].result_hero_sprite+'/'+row2[0].result_hero_type+'/'+row2[0].result_hero_count+'/'+row2[0].result_hero_name+'/';
              });

              sql = 'select count(*) as c from mix_hero_result as r, mix_hero_material as m where r.num = ? and r.num = m.num';

              var count = 0;
              connection.query(sql, [data[1]], function(err2, row2, fields2) {
                count = row2[0].c;
              });

              sql = 'select m.* from mix_hero_result as r, mix_hero_material as m where r.num = ? and r.num = m.num';

              connection.query(sql, [data[1]], function(err2, row2, fields2) {
                var i = 0;

                temp += count+'/';

                for (i = 0; i < count; i++) {
                  temp += row2[i].mat_hero_sprite+'/'+row2[i].mat_hero_type+'/'+row2[i].mat_hero_count+'/'+row2[i].mat_hero_name+'/';
                }

                console.log('success mix_hero : '+temp);
                res.end(temp);
              });
            }
          });
        }
        else if (data[0] == 'gameover') {
          var sql = 'select * from room_info where id = ?';

          connection.query(sql, [data[1]], function(err1, row1, fields1) {
            sql = 'select * from room_info where num = ?';

            connection.query(sql, [row1[0].num], function(err2, row2, fields2) {
              sql = 'select count(*) as c from room_info where num = ?';

              connection.query(sql, [row1[0].num], function(err3, row3, fields3) {
                var i = 0;

                sql = 'delete from help_unit where orig_id = ?';

                connection.query(sql, [row2[0].id], function() {});
                connection.query(sql, [row2[1].id], function() {});
                connection.query(sql, [row2[2].id], function() {});
                connection.query(sql, [row2[3].id], function() {});
              });
            });
            sql = 'delete from room_list where num = ?';

            connection.query(sql, [row1[0].num], function() {});
            sql = 'delete from boss_room where num = ?';

            connection.query(sql, [row1[0].num], function() {});
            sql = 'delete from room_info where num = ?';

            connection.query(sql, [row1[0].num], function() {});
          });

          console.log('gameover');
          res.end('');
        }
        else if (data[0] == 'gettime') {
          var sql = 'select count(*) as c from room_info as i, room_list as l where i.id = ? and i.num = l.num';

          connection.query(sql, [data[1]], function(err, row, fields) {
            if (row[0].c == 0) {
              console.log('success client gameover');
              res.end('gameover');
            }
            else {
              sql = 'select l.* from room_info as i, room_list as l where i.id = ? and i.num = l.num';

              connection.query(sql, [data[1]], function(err, row, fields) {
                console.log('success client time');
                res.end('time/'+row[0].time+'/'+row[0].stage);
              });
            }
          });
        }
        else if (data[0] == 'create_boss') {
          var sql = 'select * from boss_info where num = ?';

          connection.query(sql, [data[1]], function(err1, row1, fields1) {
            sql = 'select * from room_info where id = ?';

            connection.query(sql, [data[2]], function(err2, row2, fields2) {
              sql = 'delete from boss_room where num = ?';

              connection.query(sql, [row2[0].num], function() {});

              sql = 'insert into boss_room values (?, ?, ?)';

              connection.query(sql, [row2[0].num, row1[0].hp, row1[0].name], function(){});

              sql = 'select * from boss_room where num = ?';

              connection.query(sql, [row2[0].num], function(err3, row3, fields3) {
                console.log('success create '+row2[0].num+' - '+data[1]+' boss');
                res.end('boss/'+row3[0].boss+'/'+row3[0].hp+'/10');
              });
            });
          });
        }
        else if (data[0] == 'atk_boss') {
          //보스 데미지 계산 & lock 설정...
          var sql = 'update room_info as r, boss_room as b set hp = hp - ? where r.id = ? and r.num = b.num';

          connection.query(sql, [data[1], data[2]], function() {});

          sql = 'select count(*) as c from room_info as r, boss_room as b where r.id = ? and r.num = b.num';

          connection.query(sql, [data[2]], function(err, row, fields) {
            if (row[0].c == 0) {
              console.log('success client gameover');
              res.end('gameover');
            }
            else {
              sql = 'select b.* from room_info as r, boss_room as b where r.id = ? and r.num = b.num';

              connection.query(sql, [data[2]], function(err, row, fields) {
                console.log('send boss hp');
                res.end('alive_boss/'+row[0].hp);
              });
            }
          });
        }
        else if (data[0] == 'member') {
          var sql = 'select count(*) as c from user_info where id = ?';

          connection.query(sql, [data[1]], function(err, row, fields) {
            if (row[0].c > 0) {
              console.log('already ID');
              res.end('fail_member'); //ID가 이미 있음
            }
            else {
              sql = 'insert into user_info values(?, ?)';

              connection.query(sql, [data[1], data[2]], function() {
                console.log('create user');
                res.end('success');
              });
            }
          });
        }
        else if (data[0] == 'login') {
          var sql = 'select count(*) as c from user_info where id = ? and password = ?';

          connection.query(sql, [data[1], data[2]], function(err, row, fields) {
            if (row[0].c > 0) {
              console.log('success login');
              res.end('success/'+data[1]);
            }
            else {
              console.log('fail login');
              res.end('fail_login');
            }
          });
        }
        else if (data[0] == 'single') {
          var sql = 'insert into room_list(id, person, start, level) values (?, 1, 1, "easy")';

          connection.query(sql, [data[1]], function() {});

          sql = 'select * from room_list where id = ?';

          connection.query(sql, [data[1]], function(err, row, fields) {
            sql = 'insert into room_info values(?, ?, 1, 0, 1, 0)';

            connection.query(sql, [row[0].num, data[1]], function(){});

            console.log('success single');
            res.end('success');
          });
        }
        else if (data[0] == 'join_room') {
          var sql = 'select * , count(*) as c from room_list where person < 4 and start = 0';

          connection.query(sql, [], function(err1, row1, fields1) {
            if (row1[0].c < 1) {
              sql = 'insert into room_list(id, person, start, level) values (?, 1, 0, "easy")';

              connection.query(sql, [data[1]], function(){});

              sql = 'select * from room_list where id = ?';

              connection.query(sql, [data[1]], function(err2, row2, fields2) {
                sql = 'insert into room_info values (?, ?, 1, 0, 1, 0)';

                connection.query(sql, [row2[0].num, data[1]], function() {});

                console.log('create room boss');
                res.end('room_success');
              });
            }
            else {
              sql = 'update room_list set person = person + 1 where num = ?';

              connection.query(sql, [row1[0].num], function() {});

              sql = 'insert into room_info values(?, ?, ?, 0, 1, 0)';

              connection.query(sql, [row1[0].num, data[1], row1[0].person + 1], function() {});

              console.log('join room');
              res.end('room_success');
            }
          });
        }
        else if (data[0] == 'room_person') {
          var sql = 'select l.num, l.person, i.person_num from room_info as i, room_list as l where i.id = ? and i.num = l.num';

          connection.query(sql, [data[1]], function(err, row, fields) {
            if (row[0].person == 4) {
              console.log('start Game');
              res.end('full_room/'+row[0].person_num);
            }
            else {
              console.log('not ready Game');
              res.end('empty_room');
            }
          });
        }
        else if (data[0] == 'update_monster') {
          var sql = 'update room_info set all_monster = ? where id = ?';

          connection.query(sql, [data[1], data[2]], function() {});

          sql = 'select count(*) as c from room_info where id = ?';

          connection.query(sql, [data[2]], function(err, row, fields) {
            if (row[0].c == 0) {
              console.log('success client gameover');
              res.end('gameover');
            }
            else {
              sql = 'select * from room_info where id = ?';

              connection.query(sql, [data[2]], function(err1, row1, fields1) {

                sql = 'select count(*) as c from room_info where num = ? and id != ?';

                connection.query(sql, [row1[0].num, data[2]], function(err2, row2, fields2) {
                  sql = 'select * from room_info where num = ? and id != ?';

                  connection.query(sql, [row1[0].num, data[2]], function(err3, row3, fields3) {
                    var i = 0;
                    var send = 'friend/' + row2[0].c;

                    for (i = 0; i < row2[0].c; i++) {
                      send += '/';
                      send += row3[i].id;
                      send += '/';
                      send += row3[i].all_monster;
                    }

                    console.log('update monster');
                    res.end(send);
                  });
                });
              });
            }
          });
        }
        else if (data[0] == 'atk_boss') {
          var sql = 'select count(*) as c from room_info where id = ?';

          connection.query(sql, [data[2]], function(err, row, fields) {
            if (row[0].c == 0) {
              console.log('success client gameover');
              res.end('gameover');
            }
            else {
              sql = 'select * from room_info where id = ?';

              connection.query(sql, [data[2]], function(err, row, fields) {
                sql = 'update boss_room set hp = hp - ? where num = ?';

                connection.query(sql, [data[1], row[0].num], function(){});

                console.log('atk_boss');
                res.end('');
              });
            }
          });
        }
        else if (data[0] == 'bring_help') {
          var sql = 'select * from help_unit where type = ? and count = ? and orig_id = ?';

          connection.query(sql, [data[1], data[2], data[3]], function(err, row, fields) {
            sql = 'update help_unit set bring = true where pri = ?';

            connection.query(sql, [row[0].pri], function() {});

            console.log('success bring');
            res.end('');
          });
        }
        else if (data[0] == 'give_help') {
          var sql = 'select * from room_info where id = ?';

          connection.query(sql, [data[4]], function(err1, row1, fields1) {
            sql = 'select * from room_info where num = ? and id != ?';

            connection.query(sql, [row1[0].num, data[4]], function(err2, row2, fields2) {
              sql = 'insert into help_unit (orig_id, give_id, type, count, bring) values (?, ?, ?, ?, false)';

              connection.query(sql, [data[4], row2[data[3]-1].id, data[1], data[2]], function() {});

              console.log('add give_help');
              res.end('');
            });
          });
        }
        else if (data[0] == 'update_help_unit') {
          var sql = 'select count(*) as c from help_unit where give_id = ?';

          connection.query(sql, [data[1]], function(err1, row1, fields1) {
            sql = 'select * from help_unit where give_id = ?';

            connection.query(sql, [data[1]], function(err2, row2, fields2) {
              var i = 0;
              var send = 'update_help_unit/'+row1[0].c;

              for (i = 0; i < row1[0].c; i++) {
                send += '/';
                send += row2[i].type;
                send += '/';
                send += row2[i].count;
              }

              console.log('update help unit');
              res.end(send);
            });
          });
        }
        else if (data[0] == 'check_help_unit') {
          var sql = 'delete from help_unit where bring = true';

          connection.query(sql, [], function() {});

          sql = 'select count(*) as c from help_unit where give_id = ?';

          connection.query(sql, [data[1]], function(err, row, fields) {
            console.log('check_help_unit count');
            res.end('bring_count/'+row[0].c);
          });
        }
        else if (data[0] == 'skip') {
          var sql = 'update room_info set skip = ? where id = ?';

          connection.query(sql, [data[1], data[2]], function() {});

          console.log('skip_change');
          res.end('');
        }
        else if (data[0] == 'skip_check') {
          var sql = 'select * from room_list where id = ?';

          connection.query(sql, [data[1]], function(err1, row1, fields1) {
            sql = 'select count(*) as c from room_info where num = ? and skip = true';

            connection.query(sql, [row1[0].num], function(err2, row2, fields2) {
              if (row2[0].c == 4) {
                console.log('success skip');
                res.end('skip');
              }
              else {
                console.log('not all skip');
                res.end('');
              }
            });
          });
        }
        else {
          console.log('error');
          res.end('error');
        }
    });
});
