use std::fs::File;
use std::io::prelude::*;

use std::thread;
use std::thread::JoinHandle;
use crate::complex::Complex;

// #[derive(Debug)]
#[derive(Clone, Copy)]

struct RGB {
    r : u8,
    g : u8,
    b : u8
}

impl RGB{
    pub fn new(r : u8, g : u8, b : u8) -> RGB {
        RGB{r,g,b}
    }
    pub fn format(&self) -> String {
        // plain P3 version
        format!("{} {} {}  ",self.r,self.g,self.b)
        // P6 version
        // format!("{} {} {}  ",self.r as char,self.g as char,self.b as char)
    }
}

// #[derive(Debug)]
struct MyFile{
    name : String,
    x : i32,
    y : i32,
    max : i32,
    v : Vec<RGB>
}

impl MyFile{
    pub fn new(name:&str,x:i32,y:i32,max:i32,v:Vec<RGB>) -> MyFile {
        MyFile{name:name.to_string(),x,y,max,v}
    }

    // pub fn print_out(self){
    //     println!("P3");
    //     println!("{} {}",self.x,self.y);
    //     println!("{}",self.max);
    //     for i in 0..self.y {
    //         for j in 0..self.x {
    //             let id = i*self.x+j;
    //             print!("{}",self.v[id as usize].format())
    //         }
    //         print!("\n")
    //     }
    // }
    pub fn print_file(self) -> std::io::Result<()> {
        let mut file = File::create(self.name)?;
        file.write_all(b"P3\n")?;
        file.write_all(format!("{} {}\n",self.x,self.y).as_bytes())?;
        file.write_all(format!("{}\n",self.max).as_bytes())?;
        for i in 0..self.y {
            for j in 0..self.x {
                let id = i*self.x+j;
                file.write_all(self.v[id as usize].format().as_bytes())?;
            }
            file.write_all(b"\n")?;
        }
        Ok(())
    }
}


pub struct Picture{
    f : MyFile
}

impl Picture {
    fn calculate(pixel : &mut RGB, z : Complex, func : fn(&Complex) -> Complex) {
        let mut z = z;
        let mut val = 0;
        for _attempt in 0..256 {
            if z.abs() > 2.0 {
                val = 255;//_attempt;
                break;
            }
            z = func(&z);
        }
        // *pixel = RGB::new(val,val,val);
        pixel.r = val as u8;
        pixel.g = val as u8; 
        pixel.b = val as u8;
    }

    pub fn new(name:&str ,x:i32,y:i32,func : fn(&Complex) -> Complex) -> Picture {
        let size = x*y;
        let delta_x = 2.0/x as f32;
        let delta_y = 2.0/y as f32;

        let max_thread_count = 8 as usize;
        let mut res_rgb = vec![];

        let mut threads = vec![];
        for iter in 0..size {
            let mut elem = RGB::new(0,0,0);
            if threads.len() == max_thread_count{
                let mut tmp_res = threads.into_iter().map(|h : JoinHandle<RGB>| h.join().unwrap()).collect();
                res_rgb.append(&mut tmp_res);
                threads = vec![];
            }

            let i = ((iter as i32)/x) as f32;
            let j = ((iter as i32)%x) as f32;
            threads.push(
                thread::spawn(
                    move || {
                        Self::calculate(&mut elem,
                                        Complex::new(-1.0 + j*delta_x,-1.0 + i*delta_y),
                                        func); 
                    elem}
                )
            );
        }

        let mut tmp_res = threads.into_iter().map(|h| h.join().unwrap()).collect();
        res_rgb.append(&mut tmp_res);

        Picture{f: MyFile::new(name,x,y,255,res_rgb)}
    }

    pub fn new_single(name:&str ,x:i32,y:i32,func : fn(&Complex) -> Complex) -> Picture {
        let size = x*y;
        let mut rgb_vec: Vec<RGB> = Vec::with_capacity(size as usize);
        let delta_x = 2.0/x as f32;
        let delta_y = 2.0/y as f32;
        for _i in 0..x {
            for _j in 0..y {
                rgb_vec.push(RGB::new(0,0,0));
                let id = _i*x+_j;
                Self::calculate(&mut rgb_vec[id as usize],Complex::new(-1.0 + (_j as f32)*delta_x,-1.0 + (_i as f32)*delta_y),func)
            }
        }
        Picture{f: MyFile::new(name,x,y,255,rgb_vec)}
    }

    pub fn print(self) {
        let _info = self.f.print_file();
    }
}
