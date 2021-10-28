import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.imageio.*;
import java.io.*;


public class zad extends Frame implements KeyListener{
    //region variables
    protected Random rand;
    int n = 10;

    int posX, posY;//player pos

    //images
    protected Image playerImage;
    protected Image[] wallsImages;

    node[][] tab;//maze

    MyCanvas canv;
    //frame size
    int wd, ht;
    //endregion

    class node{
        public boolean left,up,right,down;
        public node(){
            left=false;up=false;right=false;down=false;
        }
        Image draw(){
            //ALL,D,L,LD,LR,LRD,LRU,LU,LUD,R,RD,RU,RUD,U,UD
            int s = sides();

            if(s==1){
                if(left)
                    return wallsImages[2];//L
                if(right)
                    return wallsImages[9];//R
                if(up)
                    return wallsImages[13];//U
                if(down)
                    return wallsImages[1];//D
            }
            if(s==3){
                if(!left)
                    return wallsImages[12];//RUD
                if(!right)
                    return wallsImages[8];//LUD
                if(!up)
                    return wallsImages[5];//LRD
                if(!down)
                    return wallsImages[6];//LRU
            }

            //s==2
            if(left){
                if(down)
                    return wallsImages[3];//LD
                if(right)
                    return wallsImages[4];//LR
                if(up)
                    return wallsImages[7];//LU
            }
            if(right){
                if(down)
                    return wallsImages[10];//RD
                if(up)
                    return wallsImages[11];//RU
            }
            if(up && down)
                return wallsImages[14];   //UD

            //s==4
            return wallsImages[0];
        }
        int sides(){
            int ret = 0;
            if(left)
                ret++;
            if(up)
                ret++;
            if(right)
                ret++;
            if(down)
                ret++;
            return ret;
        }
        
        boolean[][] t;

        void connect(int i, int j){
            t[i][j]=true;

            boolean blocked[] = new boolean[4];//0-up, 1-left, 2-down, 3-right
            for(int k=0; k<4; k++)
                blocked[k]=false;
            int sum = 4;
            if(i==0 || tab[i][j].up || tab[i-1][j].sides()>1){
                blocked[0]=true;
                sum--;
            }
            if(j==0 || tab[i][j].left || tab[i][j-1].sides()>1){
                blocked[1]=true;
                sum--;
            }
            if(i==(n-1) || tab[i][j].down || tab[i+1][j].sides()>1){
                blocked[2]=true;
                sum--;
            }
            if(j==(n-1) || tab[i][j].right || tab[i][j+1].sides()>1){
                blocked[3]=true;
                sum--;
            }

            if(sum==0)
                return;
            if(tab[i][j].sides()>2)
                return;

            int order = rand.nextInt(3);
            int tmp = rand.nextInt(sum);
            for(int k=0; k<4; k++){
                int indx;
                if(order==0)//random order
                    indx = (1+3*k)%4;//1 0 3 2
                else 
                    if(order==1)
                        indx = (3*k)%4;//0 3 2 1
                    else
                        indx = 3-k;//3 2 1 0

                if(!blocked[indx])
                    if((--tmp)<=0){
                        if(indx==0){
                            tab[i][j].up=true;
                            tab[i-1][j].down=true;
                            connect(i-1,j);
                        }
                        if(indx==1){
                            tab[i][j].left=true;
                            tab[i][j-1].right=true;
                            connect(i,j-1);
                        }
                        if(indx==2){
                            tab[i][j].down=true;
                            tab[i+1][j].up=true;
                            connect(i+1,j);
                        }
                        if(indx==3){
                            tab[i][j].right=true;
                            tab[i][j+1].left=true;
                            connect(i,j+1);
                        }
                        break;
                    }
            }
            
        }

        void DFS(int i, int j){
            t[i][j]=true;
            if(i!=0 && tab[i][j].up && !t[i-1][j])
                DFS(i-1,j);
            if(j!=0 && tab[i][j].left && !t[i][j-1])
                DFS(i,j-1);
            if(i!=(n-1) && tab[i][j].down && !t[i+1][j])
                DFS(i+1,j);
            if(j!=(n-1) && tab[i][j].right && !t[i][j+1])
                DFS(i,j+1);
        }

        public void setMaze(){
            t = new boolean[n][n];
            tab = new node[n][n];
            for(int i=0; i<n; i++)
                for(int j=0; j<n; j++){
                    tab[i][j] = new node();
                    t[i][j] = false;
                }
            
            for(int i=0; i<n; i++)
                for(int j=0; j<n; j++)
                    if(!t[i][j])
                        connect(i,j);

            t = new boolean[n][n];
            for(int i=0; i<n; i++)
                for(int j=0; j<n; j++)
                    t[i][j] = false;

            DFS(0,0);
            for(int i=0; i<n; i++)
                for(int j=0; j<n; j++)
                    if(!t[i][j]){
                        if(i!=0 && !tab[i][j].up && t[i-1][j]){
                            tab[i][j].up=true;
                            tab[i-1][j].down=true;
                            DFS(i,j);
                            continue;
                        }
                        if(j!=0 && !tab[i][j].left && t[i][j-1]){
                            tab[i][j].left=true;
                            tab[i][j-1].right=true;
                            DFS(i,j);
                            continue;
                        }
                        if(i!=(n-1) && !tab[i][j].down && t[i+1][j]){
                            tab[i][j].down=true;
                            tab[i+1][j].up=true;
                            DFS(i,j);
                            continue;
                        }
                        if(j!=(n-1) && !tab[i][j].right && t[i][j+1]){
                            tab[i][j].right=true;
                            tab[i][j+1].left=true;
                            DFS(i,j);
                            continue;
                        }
                    }

        }

    }

    zad(){//init all
        gameInit();
        awtInit();
        mainLoop();
    }

    void imageInit() throws IOException{
        //ALL,D,L,LD,LR,LRD,LRU,LU,LUD,R,RD,RU,RUD,U,UD
        playerImage = ImageIO.read(new File("images/Player.png"));
        wallsImages = new Image[15];
        wallsImages[ 0] = ImageIO.read(new File("images/ALL.png"));
        wallsImages[ 1] = ImageIO.read(new File("images/D.png"));
        wallsImages[ 2] = ImageIO.read(new File("images/L.png"));
        wallsImages[ 3] = ImageIO.read(new File("images/LD.png"));
        wallsImages[ 4] = ImageIO.read(new File("images/LR.png"));
        wallsImages[ 5] = ImageIO.read(new File("images/LRD.png"));
        wallsImages[ 6] = ImageIO.read(new File("images/LRU.png"));
        wallsImages[ 7] = ImageIO.read(new File("images/LU.png"));
        wallsImages[ 8] = ImageIO.read(new File("images/LUD.png"));
        wallsImages[ 9] = ImageIO.read(new File("images/R.png"));
        wallsImages[10] = ImageIO.read(new File("images/RD.png"));
        wallsImages[11] = ImageIO.read(new File("images/RU.png"));
        wallsImages[12] = ImageIO.read(new File("images/RUD.png"));
        wallsImages[13] = ImageIO.read(new File("images/U.png"));
        wallsImages[14] = ImageIO.read(new File("images/UD.png"));
    }

    void awtInit(){
        try{
            imageInit();
        }catch(Exception e){
            System.out.println(e);
        }
        addWindowListener(new WindowAdapter(){//close window
            public void windowClosing(WindowEvent e) {
                System.exit(0);
            }
        });
        canv = new MyCanvas();
        add(canv);
        addKeyListener(this);//input
        setSize(1000,1000);
        setLayout(null);
        setVisible(true);
    }

    void gameInit(){
        rand = new Random();
        do{
            posX = rand.nextInt(10);
            posY = rand.nextInt(10);//nextInt(n) <=> rand int % n
        }while(posX==0 && posY==0);
        (new node()).setMaze();
    }

    void updateGraphics(){
        Graphics canvG = canv.getGraphics();
        int dist = Math.min(wd,ht)/n;

        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++)
                canvG.drawImage(tab[i][j].draw().getScaledInstance(dist,dist,java.awt.Image.SCALE_FAST),
                                j*dist,
                                i*dist,
                                null);
        
        canvG.drawImage(playerImage.getScaledInstance(dist/2,dist/2,java.awt.Image.SCALE_FAST),
                        (dist/4)+posX*dist,
                        (dist/4)+posY*dist,
                        null);
    }

    void isOver(){
        if(posX==0 && posY==0)//end game
            System.exit(0);
    }

    void movement(int e){
        boolean moved = false;
        if(e==KeyEvent.VK_W || e==KeyEvent.VK_UP){//UP
            if(!(posY==0))
                if(tab[posY][posX].up){
                    posY--;
                    moved = true;
                }
        }
        if(e==KeyEvent.VK_A || e==KeyEvent.VK_LEFT){//LEFT
            if(!(posX==0))
                if(tab[posY][posX].left){
                    posX--;
                    moved = true;
                }
        }
        if(e==KeyEvent.VK_S || e==KeyEvent.VK_DOWN){//DOWN
            if(!(posY==(n-1)))
                if(tab[posY][posX].down){
                    posY++;
                    moved = true;
                }
        }
        if(e==KeyEvent.VK_D || e==KeyEvent.VK_RIGHT){//RIGHT
            if(!(posX==(n-1)))
                if(tab[posY][posX].right){
                    posX++;
                    moved = true;
                }
        }

        if(moved)
            updateGraphics();

        if(e==KeyEvent.VK_ESCAPE)//EXIT GAME
            System.exit(0);
    }

    //region input
    public void keyPressed(KeyEvent e){
        movement(e.getKeyCode());
        // System.out.println("Key Pressed");
    }
    public void keyReleased(KeyEvent e){
        // System.out.println("Key Released");
    }
    public void keyTyped(KeyEvent e){
        // System.out.println("Key Typed");
    }
    //endregion

    void mainLoop(){
        while(true){
            //get size of frame, center in (wd/2,ht/2)
            wd = getSize().width;
            ht = getSize().height;
            
            int xy = Math.min(wd,ht);
            canv.setSize(xy,xy);

            isOver();
        }
    }

    class MyCanvas extends Canvas{
        public MyCanvas(){
        }
        public void paint(Graphics g){//first draw
            updateGraphics();
        }
    }

    public static void main(String[] args) {
        new zad();
    }
}
