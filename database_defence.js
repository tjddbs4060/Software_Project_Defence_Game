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
            res.end('hero/'+row[0].name+'/'+row[0].type+'/'+row[0].count+'/'+row[0].speed+'/'+row[0].atk_range+'/'+row[0].damage);
          });
        }
        else if(data[0] == 'monster') {
          var sql = 'select * from monster_info where stage = ?';

          connection.query(sql, [data[1]], function(err, row, fields) {
            res.end('monster/'+row[0].hp+'/'+row[0].def);
          });
        }
        else if (data[0] == 'createtable') {
          var sql = 'select * from room_list order by num desc';

          connection.query(sql, function(err, row, fields) {
            sql = 'create table ?_join_person(id char(15) not null, hero_type char(5), hero_count int(5), origin_id char(15) not null)';

            connection.query(sql, [row[0].num+1], function(err, row, fields) {});

            sql = 'create table ?_room_boss(hero_type char(5), hero_count int(5), id char(15), boss_hp float(10))'
            connection.query(sql, [row[0].num+1], function(err, row, fields) {});

            sql = 'create table ?_room_monster(id char(15) not null, monster_hp float(10), xpos float(10), ypos float(10))'
            connection.query(sql, [row[0].num+1], function(err, row, fields) {});

            sql = 'insert into room_list values(?, 1, false)';
            connection.query(sql, [row[0].num+1], function(err, row, fields) {});
          });
        }
        else {
          res.end('error');
        }
    });
});
