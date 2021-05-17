const express = require('express');
const fs = require('fs');
const path = require('path');
const sharp = require('sharp');
const ejs=require('ejs');
const { exec } = require("child_process");

var app = express();

app.set("view engine" , "ejs");
app.set('views', __dirname + '/views');

app.get("*/galerie.json", function(req, res){
    res.status(403).render("pagini/403");
    res.end();
});

function verificaImagini(){
	var textFisier=fs.readFileSync("resurse/json/galerie.json")
	var jsi=JSON.parse(textFisier);
    var d = new Date();
    var n = d.getMonth();
    //var n = 0;
    var cnt = 0;
    var luna = "";

    if(n==0) luna = "ianuarie";
    if(n==1) luna = "februarie";
    if(n==2) luna = "martie";
    if(n==3) luna = "aprilie";
    if(n==4) luna = "mai";
    if(n==5) luna = "iunie";
    if(n==6) luna = "iulie";
    if(n==7) luna = "august";
    if(n==8) luna = "septembrie";
    if(n==9) luna = "octombrie";
    if(n==10) luna = "noiembrie";
    if(n==11) luna = "decembrie";


	var caleGalerie=jsi.cale_galerie;
    let vectImagini=[]

    for (let im of jsi.imagini){
        for (let l of im.luni)
        {
            if(l == luna){
                cnt++;
                break;
            }
        }
	}

    console.log("cnt inainte e ", cnt);

    if(cnt>12){cnt = 12;}
    if(cnt<12 && cnt%3==1){cnt=cnt-1;}
    if(cnt<12 && cnt%3==2){cnt=cnt-2;}

    console.log("cnt", cnt);

	for (let im of jsi.imagini){
		var imVeche= path.join(caleGalerie, im.fisier);
		var ext = path.extname(im.fisier);
		var numeFisier =path.basename(im.fisier,ext)
		let imNoua=path.join(caleGalerie+"/mic/", numeFisier+"-mic"+".webp");
        
    console.log("cnt", cnt);
        if(cnt==0)
            break;
        
        for (let l of im.luni)
        {
            if(l == luna){
                vectImagini.push({mare:imVeche, mic:imNoua, titlu:im.titlu, alt:im.alt, text:im.text, luni:im.luni});
                cnt=cnt-1;
                break;
            }

        }
        console.log("cnt", cnt);
        //vectImagini.push({mare:imVeche, mic:imNoua, titlu:im.titlu, alt:im.alt, text:im.text, luni:im.luni});
		if (!fs.existsSync(imNoua))//daca nu exista imaginea, mai jos o voi crea
		sharp(imVeche)
		  .resize(150) //daca dau doar width(primul param) atunci height-ul e proportional
		  .toFile(imNoua, function(err) {
              if(err)
			    console.log("eroare conversie",imVeche, "->", imNoua, err);
		  });
	}
   
    return vectImagini;
}

function verificaImaginianim(){
	var textFisier=fs.readFileSync("resurse/json/galerie.json")
	var jsi=JSON.parse(textFisier);

	var caleGalerie=jsi.cale_galerie;
    let vectImaginianim=[]

	for (let im of jsi.imagini){
		var imVeche= path.join(caleGalerie, im.fisier);
		var ext = path.extname(im.fisier);
		var numeFisier =path.basename(im.fisier,ext)
		let imNoua=path.join(caleGalerie+"/mic/", numeFisier+"-mic"+".webp");

        vectImaginianim.push({mare:imVeche, mic:imNoua, titlu:im.titlu, alt:im.alt, text:im.text, luni:im.luni});

        //vectImagini.push({mare:imVeche, mic:imNoua, titlu:im.titlu, alt:im.alt, text:im.text, luni:im.luni});
		if (!fs.existsSync(imNoua))//daca nu exista imaginea, mai jos o voi crea
		sharp(imVeche)
		  .resize(150) //daca dau doar width(primul param) atunci height-ul e proportional
		  .toFile(imNoua, function(err) {
              if(err)
			    console.log("eroare conversie",imVeche, "->", imNoua, err);
		  });
	}
   
    return vectImaginianim;
}


console.log("Proiectul se afla la ",__dirname);

app.use("/resurse", express.static(__dirname+"/resurse"));

app.get(["/","/index"], function(req, res){
    res.render("pagini/index", {pictures: verificaImagini(), picturesanim: verificaImaginianim(), ip: req.ip});
});

app.get("/css/galerie-animata.css",function(req, res){
    /*Atentie modul de rezolvare din acest app.get() este strict pentru a demonstra niste tehnici
    si nu pentru ca ar fi cel mai eficient mod de rezolvare*/
    res.setHeader("Content-Type","text/css");//pregatesc raspunsul de tip css
    let sirScss=fs.readFileSync("./resurse/scss/galerie_animata.scss").toString("utf-8");//citesc scss-ul cs string
    numere=[7,8,9,11];
    let nrAleator =numere[Math.floor(Math.random()*numere.length)];//iau o culoare aleatoare pentru border
    let rezScss=ejs.render(sirScss,{nrPoze:nrAleator});// transmit culoarea catre scss si obtin sirul cu scss-ul compilat
    //console.log(rezScss);
    console.log("!!!!!!!!!!!!!!!!!!!!!!!");
    fs.writeFileSync("./temp/galerie-animata.scss",rezScss);//scriu scss-ul intr-un fisier temporar
    exec("sass ./temp/galerie-animata.scss ./temp/galerie-animata.css", (error, stdout, stderr) => {//execut comanda sass (asa cum am executa in cmd sau PowerShell)
        if (error) {
            console.log(`error: ${error.message}`);
            res.end();//termin transmisiunea in caz de eroare
            return;
        }
        if (stderr) {
            console.log(`stderr: ${stderr}`);
            res.end();
            return;
        }
        //console.log(`stdout: ${stdout}`);
        //totul a fost bine, trimit fisierul rezultat din compilarea scss
        res.sendFile(path.join(__dirname,"/temp/galerie-animata.css"));
    });

});



app.get("/ceva", function(req, res){
    res.setHeader("Content-Type", "text/html");
    res.write("<!DOCTYPE html><html><head><title>ceva</title></head><body>" + new Date() + "</body></html>");

});


app.get("/favico.ico", function(req , res){

});


app.get("/*", function(req, res){
    res.render("pagini"+req.url, function(err, rezultatRandare){
        if(err){
            if(err.message.includes("Failed to lookup view")){
                res.status(404).render("pagini/404");
            }
            else {
                throw err;
            }
        }
        else{
            res.send(rezultatRandare);
        }

    })
    console.log("Cerere generala")
    res.end();
});

verificaImagini();

console.log("altceva");
app.listen(8080);
console.log("Serverul a pornit!");