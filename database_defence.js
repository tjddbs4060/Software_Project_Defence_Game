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
        else {
          res.end('error');
        }
    });
});
