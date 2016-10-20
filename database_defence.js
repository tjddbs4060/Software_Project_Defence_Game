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
        else if (data[0] == 'start') {
          var sql = 'update room_list set start = true where id = ?';

          connection.query(sql, [data[1]], function() {
            console.log('start');
            res.end('start');
          });
        }
        else {
          console.log('error');
          res.end('error');
        }
    });
});
