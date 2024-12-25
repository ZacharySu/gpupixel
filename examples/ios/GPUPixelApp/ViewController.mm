//
//  ViewController.m
//  GPUPixelDemo
//
//  Created by PixPark on 2021/3/31.
//
//

#import "ViewController.h"
#import <gpupixel/gpupixel.h>

using namespace gpupixel;

@interface ViewController (){
    GPUPixelView *_gpuPixelView;
    std::shared_ptr<SourceImage> _gpuSourceImage1;
    std::shared_ptr<SourceImage> _gpuSourceImage2;
    
    std::shared_ptr<PolkaDotsCurtainTransition> _transition;
}
@property (weak, nonatomic) IBOutlet UISlider *progressSlider;
@property (weak, nonatomic) IBOutlet UILabel *progressLable;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
//    self.navigationController.navigationBarHidden = false;
    
    // init video filter
    [self initVideoFilter];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    _transition->setProgress(_progressSlider.value);
    _gpuSourceImage1->Render();
    _gpuSourceImage2->Render();
}
- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
}
- (IBAction)sliderAction:(UISlider *)sender {
    if ([sender isEqual:_progressSlider]) {
        _progressLable.text = [NSString stringWithFormat:@"%f", _progressSlider.value];
        _transition->setProgress(_progressSlider.value);
        _gpuSourceImage1->Render();
        _gpuSourceImage2->Render();
    }
}
-(void) initVideoFilter {
  gpupixel::GPUPixelContext::getInstance()->runSync([&] {
    _gpuPixelView = [[GPUPixelView alloc] initWithFrame:self.view.frame];
    [self.view addSubview:_gpuPixelView];
    [self.view sendSubviewToBack:_gpuPixelView];
    
    _transition = PolkaDotsCurtainTransition::create();
    
    NSString *imagePath = [[NSBundle mainBundle] pathForResource:@"bigbuckbunny_snapshot1" ofType:@"jpg"];
    _gpuSourceImage1 = SourceImage::create([imagePath UTF8String]);
      
      imagePath = [[NSBundle mainBundle] pathForResource:@"hBd6EPoQT2C8VQYv65ys_White_Sands" ofType:@"jpg"];
      _gpuSourceImage2 =SourceImage::create([imagePath UTF8String]);
    // filter pipline
      _gpuSourceImage1->addTarget(_transition, 0);
      _gpuSourceImage2->addTarget(_transition, 1);
      _transition->addTarget(_gpuPixelView);
      _transition->setCenter(Vector2(0.5, 0.5));
    [_gpuPixelView setBackgroundColor:[UIColor grayColor]];
    [_gpuPixelView setFillMode:(gpupixel::TargetView::PreserveAspectRatio)];
  
  });
}
@end
