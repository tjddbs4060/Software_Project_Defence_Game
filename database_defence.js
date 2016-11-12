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
        else if(data[0] == 'monster') {
          var sql = 'select * from monster_info where stage = ?';

          connection.query(sql, [data[1]], function(err, row, fields) {
            res.end('monster/'+row[0].hp+'/'+row[0].def);
          });
        }
        else if (data[0] == 'create_room') {
          var sql = 'insert into room_list (id, person, level) values (?, 1, ?)';

          connection.query(sql, [data[1], data[2]], function(err1, row1, fields1) {
            sql = 'select num, id from room_list where id = ?';

            connection.query(sql, [data[1]], function(err2, row2, fields2) {
              sql = 'insert into room_info (num, id, person_num) values (?, ?, 1)';

              connection.query(sql, [row2[0].num, row2[0].id], function() {
                console.log('success create_room');
                res.end('');
              });
            });
          });
        }
        else if (data[0] == 'add_monster') {
          var sql = 'insert into room_monster values (?, ?, ?, ?)';

          connection.query(sql, [data[1], data[2], data[3], data[4]], function() {
            console.log('success add_monster');
            res.end('');
          });
        }
        else if (data[0] == 'delete_monster') {
          var sql = 'delete from room_monster where num = ?';

          connection.query(sql, [data[1]], function() {
            console.log('success delete_monster');
            res.end('');
          })
        }
        else if (data[0] == 'add_unit') {
          var sql = 'insert into room_unit values (?, ?, ?, false, ?)';

          connection.query(sql, [data[1], data[2], data[3], data[4]], function() {
            console.log('success add_unit');
            res.end('');
          });
        }
        else if (data[0] == 'time') {
          var sql = 'update room_list set time = ? where id = ?';

          connection.query(sql, [data[1], data[2]], function() {
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
          var sql = 'delte from room_monster where id = ?';
          connection.query(sql, [data[1]], function(){});

          sql = 'delete from room_unit where origin = ?';
          connection.query(sql, [data[1]], function(){});

          sql = 'delete from boss_room where id = ?';
          connection.query(sql, [data[1]], function(){});

          sql = 'delete from room_info where id = ?';
          connection.query(sql, [data[1]], function(){});

          sql = 'select * from room_info where id = ?';
          connection.query(sql, [data[1]], function(err, row, fields) {
            if (row[0].person_num == 1) {
              sql = 'delete from room_list where id = ?';
              connection.query(sql, [data[1]], function(){});
            }
          });

          console.log('success delete ' + data[1]);
          res.end('');
        }
        else if (data[0] == 'alive_boss') {
          var sql = 'select * from room_info where id = ?';

          connection.query(sql, [data[1]], function(err, row, fields) {
            var num = row[0].num;

            sql = 'delete from boss_room where num = ?';
            connection.query(sql, [num], function(){});

            sql = 'delete from room_info where num = ?';
            connection.query(sql, [num], function(){});

            sql = 'delete from room_list where num = ?';
            connection.query(sql, [num], function(){});

            sql = 'delete from room_monster where num = ?';
            connection.query(sql, [num], function(){});

            sql = 'delete from room_unit where num = ?';
            connection.query(sql, [num], function(){});

            console.log('success delete boss');
            res.end('');
          });
        }
        else if (data[0] == 'create_boss') {
          var sql = 'select * from boss_info where num = ?';

          connection.query(sql, [data[1]], function(err1, row1, fields1) {
            sql = 'select * from room_info where id = ?';

            connection.query(sql, [data[2]], function(err2, row2, fields2) {
              sql = 'insert into boss_room values (?, ?, ?, ?)';

              connection.query(sql, [row2[0].num, row1[0].hp, row1[0].def, row1[0].name], function(){});

              sql = 'select * from boss_room where num = ?';

              connection.query(sql, [row2[0].num], function(err3, row3, fields3) {
                console.log('success create '+row2[0].num+' - '+data[1]+' boss');
                res.end('boss/'+row3[0].boss+'/'+row3[0].hp+'/'+row3[0].def);
              });
            });
          });
        }
        else if (data[0] == 'damage_boss') {
          //보스 데미지 계산 & lock 설정...
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

          connection.query(sql, [data[1]], function() {
            sql = 'select * from room_list where id = ?';

            connection.query(sql, [data[1]], function(err, row, fields) {
              sql = 'insert into room_info values(?, ?, 1, 0, 1)';

              connection.query(sql, [row[0].num, data[1]], function(){});

              console.log('success single');
              res.end('success');
            });
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
                sql = 'insert into room_info values (?, ?, 1, 0, 1)';

                connection.query(sql, [row2[0].num, data[1]], function() {});

                console.log('create room boss');
                res.end('room_success');
              });
            }
            else {
              sql = 'update room_list set person = person + 1 where num = ?';

              connection.query(sql, [row1[0].num], function() {});

              sql = 'insert into room_info values(?, ?, ?, 0, 1)';

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
        else {
          console.log('error');
          res.end('error');
        }
    });
});
