//
//  _015HW_coverage.m
//  6015HW_coverage
//
//  Created by Jinny Jeng on 2/13/23.
//

#import <XCTest/XCTest.h>
#include "run.hpp"

@interface test : XCTestCase
@end

@implementation test
- (void)testAll {
    if (!run_tests())
        XCTFail(@"failed");
}
@end
